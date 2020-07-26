# Copyright (c) 2019 askmeaboutloom
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
package XLua::Generator;
use 5.020;
use warnings;
use experimental qw(signatures);
use Data::Dumper;
use JSON::PP;


my %default_typemap = (
    any => {
        decl  => 'int $var',
        check => 'luaL_checkany(L, $index);\n    $decl = $index',
        push  => 'lua_pushvalue(L, $var)',
    },
    table => {
        decl  => 'int $var',
        check => 'luaL_checktype(L, $index, LUA_TTABLE);\n    $decl = $index',
        push  => 'lua_pushvalue(L, $var)',
    },
    lua_Integer => {
        decl  => 'lua_Integer $var',
        check => '$decl = luaL_checkinteger(L, $index)',
        push  => 'lua_pushinteger(L, $var)',
    },
    lua_Number => {
        decl  => 'lua_Number $var',
        check => '$decl = luaL_checknumber(L, $index)',
        push  => 'lua_pushnumber(L, $var)',
    },
    bool => {
        decl  => 'bool $var',
        check => '$decl = XL_checkbool(L, $index)',
        push  => 'lua_pushboolean(L, $var)',
    },
    int => {
        decl  => 'int $var',
        check => '$decl = XL_checkint(L, $index)',
        push  => 'XL_pushint(L, $var)',
    },
    float => {
        decl  => 'float $var',
        check => '$decl = XL_checkfloat(L, $index)',
        push  => 'XL_pushfloat(L, $var)',
    },
    uint => {
        decl  => 'unsigned int $var',
        check => '$decl = XL_checkuint(L, $index)',
        push  => 'XL_pushuint(L, $var)',
    },
    uchar => {
        decl  => 'unsigned char $var',
        check => '$decl = XL_checkuchar(L, $index)',
        push  => 'XL_pushuchar(L, $var)',
    },
    uint8_t => {
        decl  => 'uint8_t $var',
        check => '$decl = XL_checkuint8(L, $index)',
        push  => 'XL_pushuint8(L, $var)',
    },
    int32_t => {
        decl  => 'int32_t $var',
        check => '$decl = XL_checkint32(L, $index)',
        push  => 'XL_pushint32(L, $var)',
    },
    uint32_t => {
        decl  => 'uint32_t $var',
        check => '$decl = XL_checkuint32(L, $index)',
        push  => 'XL_pushuint32(L, $var)',
    },
    'constchar*' => {
        decl  => 'const char *$var',
        check => '$decl = luaL_checkstring(L, $index)',
        push  => 'lua_pushstring(L, $var)',
    },
    '<LEN>constchar*' => {
        decl  => 'size_t ${var}_len;\n    const char *$var',
        check => '$decl = luaL_checklstring(L, $index, &${var}_len)',
        push  => 'lua_pushlstring(L, $var, ${var}_len)',
    },
    'R_String*' => {
        decl  => 'R_String *$var',
        check => 'size_t ${var}_LEN;\n' .
             '    const char *${var}_STR = luaL_checklstring(L, $index, &${var}_LEN);\n' .
             '    $decl = R_string_from_bin(${var}_STR, ${var}_LEN)',
        push  => 'R_lua_push_r_string(L, $var)',
    },
);

for my $alias ([uint  => ['unsigned', 'unsignedint']],
               [uchar => ['unsignedchar']]) {
    my ($dst, $srcs) = @$alias;
    for my $src (@$srcs) {
        $default_typemap{$src} = $default_typemap{$dst};
    }
}


sub new ($class, $out) {
    return bless {
        out              => $out,
        types            => {%default_typemap},
        types_by_package => {},
        registries       => {
            function => {},
            method   => {},
            index    => {},
            newindex => {},
        },
    }, $class;
}

sub print ($self, @args) {
    print {$self->{out}} @args;
}

sub println ($self, @args) {
    $self->print(@args, "\n");
}

sub get_type ($self, $type) {
    return $self->{types}{$type} || die "Unknown type '$type'\n";
}

sub get_type_by_package ($self, $package) {
    return $self->{types_by_package}{$package}
        || die "No type for package '$package'\n";
}

sub register ($self, $kind, $package, $lua_name, $c_name) {
    if (defined $lua_name) {
        $self->{registries}{$kind}{$package}{$lua_name} = $c_name;
    }
    else {
        $self->{registries}{$kind}{$package} = $c_name;
    }
}


sub c_name ($name) {
    $name =~ s/\W/_/g;
    $name =~ s/_{2,}/_/g;
    return "\L$name\E_xl";
}


sub generate_pptype ($self, $name, $packages, $nuvalue) {
    my $decl   = sprintf '%s *$var', $name;
    my $check  = sprintf '$decl = XL_checkpptype(L, $index, \"%s\")', $name;
    my $ncheck = sprintf '$decl = XL_checkpptype_nullable(L, $index, \"%s\")', $name;
    my $new    = sprintf 'XL_pushnewpptypeuv(L, $var, \"%s\", %d)', $name, $nuvalue;
    my $new_n  = sprintf 'XL_pushnewpptypeuv_nullable(L, $var, \"%s\", %d)', $name, $nuvalue;
    my $die    = sprintf '" . die("A plain %s* can\'t be pushed\n") . "', $name;
    my %type   = (decl => $decl, check => $check, metatable => $name);

    $self->{types}{"$name*"}               = {%type, push => $die};
    $self->{types}{"<NEW>$name*"}          = {%type, push => $new};
    $self->{types}{"<NEW_NULLABLE>$name*"} = {%type, push => $new_n};
    $self->{types}{"<NULLABLE>$name*"}     = {%type, push => $die, check => $ncheck};

    for my $package (@$packages) {
        $self->{types_by_packages}{$package} = $name;
    }
}

sub generate_utype ($self, $name, $packages, $nuvalue) {
    my $decl  = sprintf '%s *$var', $name;
    my $ndecl = sprintf '%s $var', $name;
    my $check = sprintf '$decl = XL_checkutype(L, $index, \"%s\")', $name;
    my $new   = sprintf 'XL_pushnewutypeuv(L, $var, sizeof(%1$s), \"%1$s\", %2$d)', $name, $nuvalue;
    my $nnew  = sprintf 'XL_pushnewutypeuv(L, &$var, sizeof(%1$s), \"%1$s\", %2$d)', $name, $nuvalue;
    my $die   = sprintf '" . die("A plain %s* can\'t be pushed\n") . "', $name;
    my $ndie  = sprintf '" . die("A new %s* can\'t be an argument\n") . "', $name;
    my %type  = (decl => $decl, check => $check, metatable => $name);

    $self->{types}{"$name*"}      = {%type, push => $die};
    $self->{types}{"<NEW>$name*"} = {
        %type,
        decl  => $ndecl,
        check => $ndie,
        push  => $nnew,
    };
    $self->{types}{$name} = {
        metatable => $name,
        decl      => $ndecl,
        check     => sprintf(
            '$decl = *((%1$s *)luaL_checkudata(L, $index, \"%1$s\"))', $name),
        push      => $nnew,
    };

    for my $package (@$packages) {
        $self->{types_by_packages}{$package} = $name;
    }
}

sub generate_types ($self, $types) {
    for my $type (values %$types) {
        my $method = "generate_$type->{kind}";
        $self->$method(@{$type}{'name', 'packages', 'nuvalue'});
    }
}


sub generate_c_block ($self, $block) {
    $self->print($block->{content});
}


sub generate_arg ($self, $index, $type_name, $var) {
    my $type  = $self->get_type($type_name);
    my $decl  = eval qq/"$type->{decl}"/  or die $@;
    my $check = eval qq/"$type->{check}"/ or die $@;
    $self->println("    $check;");
}

sub generate_args ($self, $args) {
    # my $argc = @$args;
    # $self->println("    XL_check_argc_exact(L, $argc);");
    for my $i (0 .. $#$args) {
        my $arg = $args->[$i];
        $self->generate_arg($i + 1, @{$arg}{'type', 'name'});
    }
}

sub generate_retval ($self, $type_name) {
    my $type = $self->get_type($type_name);
    my $var  = 'RETVAL';
    my $decl = eval qq/"$type->{decl}"/ or die $@;
    $self->println("    $decl;");
}

sub generate_return ($self, $type_name, $leave) {
    if ($type_name eq 'void') {
        $self->println($leave) if $leave;
        $self->println('    return 0;');
    }
    else {
        my $type = $self->get_type($type_name);
        my $var  = 'RETVAL';
        my $push = eval qq/"$type->{push}"/ or die $@;
        $self->println("    $push;");
        $self->println($leave) if $leave;
        $self->println("    return 1;");
    }
}

sub generate_body ($self, $preamble, $bodies, $postamble, $return, $has_retval) {
    my $body  = ref $bodies ? $bodies->{body} : $bodies;
    my $leave = ref $bodies ? $bodies->{leave} : undef;

    $self->println($preamble) if $preamble;
    $self->generate_retval($return) if $has_retval;
    $self->println($body);
    $self->println($postamble) if $postamble;

    if ($return eq '...') {
        $self->println($leave) if $leave;
    }
    else {
        $self->generate_return($return, $leave);
    }
}

sub generate_function_block ($self, $block, $suffix = '', $register = undef) {
    my $block_name = $block->{name} // '';
    my $func_name  = c_name("$block->{package}.$block_name$suffix");
    $self->println("static int $func_name(lua_State *L)");
    $self->println('{');
    $self->generate_args($block->{args});
    $self->generate_body(@{$block}{qw(preamble body postamble return has_retval)});
    $self->println('}');

    my $kind    = $register || ($block->{is_method} ? 'method' : 'function');
    my $package = $block->{register} || $block->{package};
    $self->register($kind, $package, $block->{name}, $func_name);
}

sub generate_index_block ($self, $block) {
    $self->generate_function_block($block, '_index', 'index');
}

sub generate_newindex_block ($self, $block) {
    $self->generate_function_block($block, '_newindex', 'newindex');
}

sub generate_intindex_block ($self, $block) {
    $self->generate_function_block($block, '_intindex', 'intindex');
}

sub generate_intnewindex_block ($self, $block) {
    $self->generate_function_block($block, '_intnewindex', 'intnewindex');
}


sub generate_blocks ($self, $blocks) {
    for my $block (@$blocks) {
        my $type   = $block->{type};
        my $method = "generate_${type}_block";
        if ($self->can($method)) {
            $self->$method($block);
        }
        else {
            die "Don't know how to generate a block of type '$type'\n";
        }
        $self->println();
    }
}


sub generate_index_metamethod ($self, $package, $type_name, $fallback = 0) {
    my $dummy_name = c_name("$package.index_dummy");
    my $func_name  = c_name("$package.index");

    $self->println("static int $dummy_name;") unless $fallback;
    $self->println("static int $func_name(lua_State *L)");
    $self->println('{');

    if (my $intindex = $self->{registries}{intindex}{$package}) {
        $self->println('    if (lua_isinteger(L, 2)) {');
        $self->println("        return $intindex(L);");
        $self->println('    }');
    }

    if ($fallback) {
        $self->println("    return XL_index_fallback(L, \"$package\", 1, 2);");
    }
    else {
        $self->println("    return XL_index(L, \"$package\", &$dummy_name, 1, 2);");
    }

    $self->println('}');
    $self->println();

    my $type = $self->get_type($type_name);
    $self->register('method', $type->{metatable}, '__index', $func_name);
}

sub generate_index_metamethods ($self, $blocks) {
    my %packages = map { $_->{package} => $_->{args}[0]{type} }
                   grep { $_->{type} eq 'index' }
                   @$blocks;
    for my $package (sort keys %packages) {
        $self->generate_index_metamethod($package, $packages{$package});
    }
}

sub generate_fallback_index_metamethod ($self, $package) {
    $self->generate_index_metamethod($package, "$package*", 1);
}

sub generate_fallback_index_metamethods ($self) {
    my $method_registry = $self->{registries}{method};
    my @packages = grep { !$method_registry->{$_}{__index} }
                   sort keys %$method_registry;

    for my $package (@packages) {
        $self->generate_fallback_index_metamethod($package);
    }
}

sub generate_newindex_metamethod ($self, $package, $type_name) {
    my $dummy_name = c_name("$package.newindex_dummy");
    my $func_name  = c_name("$package.newindex");

    $self->println("static int $dummy_name;");
    $self->println("static int $func_name(lua_State *L)");
    $self->println('{');

    if (my $intnewindex = $self->{registries}{intnewindex}{$package}) {
        $self->println('    if (lua_isinteger(L, 2)) {');
        $self->println("        return $intnewindex(L);");
        $self->println('    }');
    }

    $self->println("    return XL_newindex(L, \"$package\", &$dummy_name, 1, 2, 3);");
    $self->println('}');
    $self->println();

    my $type = $self->get_type($type_name);
    $self->register('method', $type->{metatable}, '__newindex', $func_name);
}

sub generate_newindex_metamethods ($self, $blocks) {
    my %packages = map { $_->{package} => $_->{args}[0]{type} }
                   grep { $_->{type} eq 'newindex' }
                   @$blocks;
    for my $package (sort keys %packages) {
        $self->generate_newindex_metamethod($package, $packages{$package});
    }
}

sub generate_intnewindex_metamethod ($self, $package, $type_name) {
    my $func_name   = c_name("$package.newindex");
    my $intnewindex = $self->{registries}{intnewindex}{$package};
    $self->println("#define $func_name $intnewindex\n");

    my $type = $self->get_type($type_name);
    $self->register('method', $type->{metatable}, '__newindex', $func_name);
}

sub generate_intnewindex_metamethods ($self, $blocks) {
    my %defined = map { $_->{package} => 1 }
                  grep { $_->{type} eq 'newindex' }
                  @$blocks;
    my %packages = map { $_->{package} => $_->{args}[0]{type} }
                   grep { !$defined{$_->{package}} }
                   grep { $_->{type} eq 'intnewindex' }
                   @$blocks;
    for my $package (sort keys %packages) {
        $self->generate_intnewindex_metamethod($package, $packages{$package});
    }
}

sub generate_indexes ($self, $blocks) {
    $self->generate_index_metamethods($blocks);
    $self->generate_fallback_index_metamethods();
    $self->generate_newindex_metamethods($blocks);
    $self->generate_intnewindex_metamethods($blocks);
}


sub generate_registry ($self, $kind, $package, $entries) {
    my $registry_name = c_name("${package}_${kind}_registry");
    $self->println("static luaL_Reg $registry_name\[] = {");

    for my $lua_name (sort keys %$entries) {
        $self->println("    {\"$lua_name\", $entries->{$lua_name}},");
    }

    $self->println('    {NULL, NULL},');
    $self->println('};');
    $self->println();
}

sub generate_registries ($self) {
    for my $kind (sort keys %{$self->{registries}}) {
        for my $package (sort keys %{$self->{registries}{$kind}}) {
            my $entries = $self->{registries}{$kind}{$package};
            next unless ref $entries;
            $self->generate_registry($kind, $package, $entries);
        }
    }
}


sub generate_enums ($self, $enums) {
    for my $enum (@$enums) {
        my $c_name = c_name("$enum->{package}.$enum->{name}_enum");
        $self->println("static XL_EnumEntry $c_name\[] = {");

        for my $entry (@{$enum->{entries}}) {
            $self->println("    {\"$entry->{key}\", (lua_Integer) $entry->{value}},");
        }

        $self->println('    {NULL, (lua_Integer) 0},');
        $self->println('};');
        $self->println();
    }
}


sub generate_init_metatables ($self, $metatables) {
    my $method_registry = $self->{registries}{method};
    for my $metatable (sort keys %$method_registry, @$metatables) {
        my $registry_name = exists $method_registry->{$metatable}
                          ? c_name("${metatable}_method_registry")
                          : 'NULL';
        $self->println("    XL_initmetatable(L, \"$metatable\", $registry_name);");
    }
}

sub generate_init_indextables ($self) {
    my $index_registry = $self->{registries}{index};
    for my $package (sort keys %$index_registry) {
        my $dummy_name    = c_name("${package}_index_dummy");
        my $registry_name = c_name("${package}_index_registry");
        $self->println("    XL_initindextable(L, &$dummy_name, $registry_name);");
    }
}

sub generate_init_newindextables ($self) {
    my $newindex_registry = $self->{registries}{newindex};
    for my $package (sort keys %$newindex_registry) {
        my $dummy_name    = c_name("${package}_newindex_dummy");
        my $registry_name = c_name("${package}_newindex_registry");
        $self->println("    XL_initnewindextable(L, &$dummy_name, $registry_name);");
    }
}

sub generate_init_functions ($self) {
    my $function_registry = $self->{registries}{function};
    for my $package (sort keys %$function_registry) {
        my $registry_name = c_name("${package}_function_registry");
        my $package_args  = join ', ', map { "\"$_\"" } split /\./, $package;
        $self->println("    XL_initfunctions(L, $registry_name, ",
                       "$package_args, (const char *)NULL);");
    }
}

sub generate_init_enums ($self, $enums) {
    for my $enum (@$enums) {
        my $c_name = c_name("$enum->{package}.$enum->{name}_enum");
        my $package_args = join ', ',
                           map { "\"$_\"" }
                           (split(/\./, $enum->{package}), $enum->{name});
        $self->println("    XL_initenum(L, $c_name, $package_args, (const char *)NULL);");
    }
}

sub generate_init ($self, $linkage, $name, $enums, $metatables) {
    $self->print('static ') if $linkage eq 'static';
    $self->println("int $name(lua_State *L)");
    $self->println('{');

    $self->generate_init_metatables($metatables);
    $self->generate_init_indextables();
    $self->generate_init_newindextables();
    $self->generate_init_functions();
    $self->generate_init_enums($enums);

    $self->println('    return 0;');
    $self->println('}');
}


sub generate ($class, $out, $parse) {
    my $self = $class->new($out);
    $self->println("/*************************************************/");
    $self->println("/* This file was auto-generated by xlgen.        */");
    $self->println("/* Don't edit it directly, it'll be overwritten! */");
    $self->println("/* Modify the matching .xl file and rebuild.     */");
    $self->println("/*************************************************/");
    $self->generate_types($parse->{types});
    $self->generate_blocks($parse->{blocks});
    $self->generate_indexes($parse->{blocks});
    $self->generate_registries();
    $self->generate_enums($parse->{enums});
    $self->generate_init(@{$parse->{init}}{'linkage', 'name'},
                         $parse->{enums}, $parse->{metatables});
    return $self;
}

1;
