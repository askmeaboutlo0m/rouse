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
package XLua::Parser;
use 5.020;
use warnings;
use experimental qw(signatures);
use JSON::PP;

sub new ($class) {
    return bless {
        blocks     => [],
        enums      => [],
        types      => {},
        metatables => [],
        last_type  => undef,
        fieldcheck => undef,
        before     => undef,
        preamble   => undef,
        postamble  => undef,
        after      => undef,
        init       => {
            linkage => 'static',
            name    => 'init',
        },
    }, $class;
}

sub ambles ($self) {
    return %{$self}{qw/before preamble postamble after/};
}

sub add_block ($self, $block) {
    push @{$self->{blocks}}, $block;
    return $self;
}

sub last_type ($self) {
    return $self->{last_type} // die "No last type (missing (PP|U)TYPE?)\n";
}

sub last_package ($self) {
    my $name     = $self->last_type;
    my $packages = $self->{types}{$name}{packages};
    die "No packages for type '$name'\n" unless @$packages;
    return $packages->[-1];
}


my $at_c  = qr/\A\s*\@\s*c\s*\z/;
my $at_xl = qr/\A\s*\@\s*xl\s*\z/;

sub parse_c ($self, $in) {
    my $content = '';
    my $mode;

    while (<$in>) {
        if (/$at_c/) {
            next;
        }
        elsif (/$at_xl/) {
            $mode = 'xl';
            last;
        }
        else {
            $content .= $_;
        }
    }

    if ($content =~ /\S/) {
        $self->add_block({
            type    => 'c',
            content => $content,
        });
    }

    return $mode;
}


my $package_qr = qr/\w+(?:\.\w+)+/;
my $return_qr  = qr/.*?\S.*?/;

sub parse_type ($self, $kind, $in, $line) {
    $line =~ /\A(\w+)(?:\s+($package_qr))?(?:\s+([0-9]+))?\s*/
        or die "Can't parse \U$kind\E '$line'\n";
    my ($name, $package, $nuvalue) = ($1, $2, $3);
    my $type = $self->{types}{$name} ||= {
        kind     => $kind,
        name     => $name,
        packages => [],
        nuvalue  => 0 + ($3 // 0),
    };
    die "Redeclaration of type '$name' as a $kind\n" if $type->{kind} ne $kind;
    push @{$type->{packages}}, $package if $package;
    $self->{last_type} = $name;
}

sub parse_pptype_directive ($self, $in, $line) {
    $self->parse_type('pptype', $in, $line);
}

sub parse_utype_directive ($self, $in, $line) {
    $self->parse_type('utype', $in, $line);
}


sub normalize_type ($type) {
    $type =~ s/\A\s+|\s+\z//g;
    $type =~ s/\s//g;
    return $type;
}

sub decipher_return ($prefix) {
    my $return     = normalize_type($prefix);
    my $has_retval = $return ne 'void' && $return ne '...';
    return ($return, $has_retval ? JSON::PP::true : JSON::PP::false);
}


sub parse_arg ($self, $text) {
    my ($type, $name) = $text =~ /\A(.+?)\s*(\w+)\z/;
    return {
        type => normalize_type($type),
        name => $name,
    };
}

sub parse_args ($self, $in, $text) {
    while ($text !~ /\)/) {
        my $next = <$in>;
        $next =~ s/\A\s+|\s+\z//g;
        $text .= ' ' . $next;
    }
    my ($arglist, $after_args) = $text =~ /\A\s*\(([^\)]*)\)\s*(.*)/s
        or die "Can't parse arglist: $text\n";

    my @args = map { $self->parse_arg($_) }
               map { s/\s+/ /gr }
               map { s/\A\s+|\s+\z//r }
               grep { /\S/ }
               split /\s*,\s*/, $arglist;

    return (\@args, $after_args);
}

sub parse_until ($self, $in, $qr) {
    my $body = '';
    while (<$in>) {
        if (/\A\s*($qr)\s*\z/) {
            my $type = "$1";
            $body =~ s/\s+\z//;
            return ($type, $body);
        }
        else {
            $body .= $_;
        }
    }
    die "Didn't find $qr before end of file\n";
}

sub parse_until_leave_or_end ($self, $in) {
    return parse_until($self, $in, qr/LEAVE|END/);
}

sub parse_until_end ($self, $in) {
    return (parse_until($self, $in, qr/END/))[1];
}

sub parse_body ($self, $in, $has_retval, $args, $after_args) {
    if ($after_args =~ /\A\s*=\s*(\w+)\s*\z/) {
        my $call = sprintf '%s(%s)', $1, join ', ', map { $_->{name} } @$args;
        return $has_retval ? "    RETVAL = $call;" : "    $call;";
    }
    elsif ($after_args =~ /\A\h*\S/)  {
        die "garbage after arguments: '$after_args'\n";
    }
    elsif ($after_args =~ /\n\s*END\s*\z/) {
        $after_args =~ s/\A\s*\n//;
        $after_args =~ s/\s*END\s*\z//;
        return $after_args;
    }

    my ($type, $body) = $self->parse_until_leave_or_end($in);
    return $body if $type eq 'END';

    return {body => $body, leave => $self->parse_until_end($in)};
}

sub parse_function_directive ($self, $in, $line) {
    my ($prefix, $location, $rest) = $line =~ /\A($return_qr)\s*($package_qr|\w+:\w+)\s*(\(.+)\z/s
        or die "Don't understand this function declaration: $line";

    my ($register, $name) = $location =~ /\A(.*?)[\.:]?(\w+)\z/;
    my $is_method         = $location =~ /:/ ? JSON::PP::true : JSON::PP::false;
    my $package           = $is_method ? "${register}_method" : $register;

    my ($return, $has_retval) = decipher_return($prefix);
    my ($args, $after_args)   = $self->parse_args($in, $rest);
    my $body = $self->parse_body($in, $has_retval, $args, $after_args);
    $self->add_block({
        type       => 'function',
        return     => $return,
        has_retval => $has_retval,
        package    => $package,
        register   => $register,
        is_method  => $is_method,
        name       => $name,
        args       => $args,
        body       => $body,
        $self->ambles,
    });
}

sub parse_constructor_directive ($self, $in, $line) {
    my ($name, $rest) = $line =~ /\A(\w+)\s*(\(.*)\z/s
        or die "Can't parse CONSTRUCTOR '$line'\n";
    my $return  = $self->last_type;
    my $package = $self->last_package;
    $self->parse_function_directive($in, "<NEW>$return *$package.$name$rest");
}

sub parse_method_directive ($self, $in, $line) {
    my ($return, $name, $rest) = $line =~ /\A($return_qr)\s*(\b\w+\b)\s*\((.*)\z/s
        or die "Can't parse METHOD '$line'\n";
    my $callee = $self->last_type;
    $self->parse_function_directive($in,
        "$return $callee:$name($callee *self, $rest");
}

sub parse_destructor_directive ($self, $in, $line) {
    my $type = $self->last_type;
    $self->parse_function_directive($in, "void $type:__gc(<NULLABLE>$type *self)$line");
}


sub parse_index_directive ($self, $in, $line) {
    my ($prefix, $name, $rest);

    if ($line =~ /\A($return_qr)\s*\b(\w+)\b\s*(=\s*\w+)\s*\z/) {
        ($prefix, $name, $rest) = ($1, $2, $3);
    }
    elsif ($line =~ /\A($return_qr)\s*\b(\w+)\b\h*$/mp) {
        ($prefix, $name, $rest) = ($1, $2, ${^POSTMATCH});
    }
    else {
        die "Don't understand this index declaration: $line\n";
    }

    my ($return, $has_retval) = decipher_return($prefix);

    my $callee = $self->last_type;
    my ($args) = $self->parse_args(undef, "($callee *self)");
    my $body   = $self->parse_body($in, $has_retval, $args, $rest);

    $self->add_block({
        type       => 'index',
        return     => $return,
        has_retval => $has_retval,
        name       => $name,
        package    => $callee,
        is_method  => JSON::PP::true,
        args       => $args,
        body       => $body,
        $self->ambles,
    });
}

sub parse_newindex_directive ($self, $in, $line) {
    my ($prefix, $name, $rest);

    if ($line =~ /\A($return_qr)\s*\b(\w+)\b\s*(=\s*\w+)\s*\z/) {
        ($prefix, $name, $rest) = ($1, $2, $3);
    }
    elsif ($line =~ /\A($return_qr)\s*\b(\w+)\b\h*$/mp) {
        ($prefix, $name, $rest) = ($1, $2, ${^POSTMATCH});
    }
    else {
        die "Don't understand this newindex declaration: $line\n";
    }

    my $callee = $self->last_type;
    my ($args) = $self->parse_args(undef, "($callee *self, $prefix VALUE)");
    my $body   = $self->parse_body($in, 0, $args, $rest);

    $self->add_block({
        type       => 'newindex',
        return     => 'void',
        has_retval => 0,
        name       => $name,
        package    => $callee,
        is_method  => JSON::PP::true,
        args       => $args,
        body       => $body,
        $self->ambles,
    });
}

sub parse_intindex_directive ($self, $in, $line) {
    my ($return, $has_retval) = decipher_return($line);

    my $callee = $self->last_type;
    my ($args) = $self->parse_args(undef, "($callee *self, lua_Integer INDEX)");
    my $body   = $self->parse_body($in, $has_retval, $args, '');

    $self->add_block({
        type       => 'intindex',
        return     => $return,
        has_retval => $has_retval,
        package    => $callee,
        is_method  => JSON::PP::true,
        args       => $args,
        body       => $body,
        $self->ambles,
    });
}

sub parse_intnewindex_directive ($self, $in, $line) {
    my $callee = $self->last_type;
    my ($args) = $self->parse_args(
            undef, "($callee *self, lua_Integer INDEX, $line VALUE)");
    my $body   = $self->parse_body($in, 0, $args, '');

    $self->add_block({
        type       => 'intnewindex',
        return     => 'void',
        has_retval => 0,
        package    => $callee,
        is_method  => JSON::PP::true,
        args       => $args,
        body       => $body,
        $self->ambles,
    });
}

sub parse_setfromtable_directive ($self, $in, $line) {
    $line =~ /\A\w+\z/ or die "Can't parse SETFROMTABLE: '$line'\n";
    my $callee = $self->last_type;
    my $body   = "    return XL_setfromtable(L, \"$callee\", 1, 2);\nEND";
    open my $pseudo_in, '<', \$body;
    $self->parse_function_directive($pseudo_in, "... $callee:$line()");
}


sub parse_fieldcheck_directive ($self, $in, $line) {
    $self->{fieldcheck} = $line =~ /\S/ ? $line : undef;
}

sub parse_amble ($self, $kind, $in, $line) {
    if ($line =~ /\A=\s*(.*?)\s*\z/) {
        return $1;
    }
    elsif ($line !~ /\S/) {
        return $self->parse_until_end($in);
    }
    else {
        die "Garbage after $kind directive: '$line'\n";
    }
}

sub parse_before_directive ($self, $in, $line) {
    $self->{before} = $self->parse_amble('BEFORE', $in, $line);
}

sub parse_preamble_directive ($self, $in, $line) {
    $self->{preamble} = $self->parse_amble('PREAMBLE', $in, $line);
}

sub parse_postamble_directive ($self, $in, $line) {
    $self->{postamble} = $self->parse_amble('POSTAMBLE', $in, $line);
}

sub parse_after_directive ($self, $in, $line) {
    $self->{after} = $self->parse_amble('AFTER', $in, $line);
}

sub parse_indexfield_directive ($self, $in, $line) {
    my ($type, $name, $access);
    if ($line =~ /\A(.+?)\s*\b(\w+)\s*=\s*(.+?)\s*\z/s) {
        ($type, $name, $access) = ($1, $2, $3);
    }
    elsif ($line =~ /\A(.+?)\s*\b(\w+)\z/s) {
        ($type, $name, $access) = ($1, $2, $2);
    }
    else {
        die "Can't parse INDEXFIELD: '$line'\n";
    }

    my @lines = ("$type $name");

    my $fieldcheck = $self->{fieldcheck};
    push @lines, "    $fieldcheck;" if $fieldcheck;

    push @lines, "    RETVAL = self->$access;", 'END';
    $self->parse_index_directive(undef, join "\n", @lines);
}

sub parse_newindexfield_directive ($self, $in, $line) {
    my ($type, $name, $access);
    if ($line =~ /\A(.+?)\s*\b(\w+)\s*=\s*(.+?)\s*\z/s) {
        ($type, $name, $access) = ($1, $2, $3);
    }
    elsif ($line =~ /\A(.+?)\s*\b(\w+)\z/s) {
        ($type, $name, $access) = ($1, $2, $2);
    }
    else {
        die "Can't parse NEWINDEXFIELD: '$line'\n";
    }

    my @lines = ("$type $name");

    my $fieldcheck = $self->{fieldcheck};
    push @lines, "    $fieldcheck;" if $fieldcheck;

    push @lines, "    self->$access = VALUE;", 'END';
    $self->parse_newindex_directive(undef, join "\n", @lines);
}

sub parse_field_directive ($self, $in, $line) {
    $self->parse_indexfield_directive(undef, $line);
    $self->parse_newindexfield_directive(undef, $line);
}


sub parse_enum_directive ($self, $in, $line) {
    my ($location, $subs) = $line =~ /\A($package_qr)(?:\s+(s.+))?\z/
        or die "Can't parse ENUM: '$line'\n";

    my ($package, $name) = $location =~ /\A(.*?)\.?(\w+)\z/;
    my @entries;

    while (<$in>) {
        next unless /\S/;
        chomp;

        if (/\A\s*(\w+)\s*=\s*(.+?)\s*\z/) {
            push @entries, {key => "$1", value => "$2"};
        }
        elsif (/\A\s*END\s*\z/) {
            push @{$self->{enums}}, {
                type    => 'enum',
                package => $package,
                name    => $name,
                entries => \@entries,
            };
            return;
        }
        elsif (/\A\s*(\w+)\s*\z/) {
            my $value = "$1";
            my $key   = $value;
            eval "\$key =~ $subs";
            die "error in enum substitution '$subs': $@" if $@;
            push @entries, {key => $key, value => $value};
        }
        else {
            die "Can't parse enum value: '$_'\n";
        }
    }

    die "Didn't find an 'END' directive before end of file\n";
}


sub parse_metatable_directive ($self, $in, $line) {
    my ($name) = $line =~ /\A(\w+)\z/
        or die "Can't parse METATABLE directive: $line\n";
    push @{$self->{metatables}}, $name;
}


sub parse_init_directive ($self, $in, $line) {
    my ($linkage, $name) = $line =~ /\A(extern|static)\s+(\w+)\z/
        or die "Can't parse INIT directive: $line\n";
    $self->{init} = {
        linkage => $linkage,
        name    => $name,
    };
}


sub parse_directive ($self, $in, $line) {
    $line =~ s/\A\s+|\s+\z//g;
    if ($line =~ /\A([A-Z]+)(?:\s+(.*)|\s*)\z/s) {
        my ($directive, $rest) = ($1, $2);
        my $method = "parse_\L$1\E_directive";
        if ($self->can($method)) {
            $self->$method($in, $rest // '');
        }
        else {
            die "Unknown directive '$directive'\n";
        }
    }
    else {
        die "No directive on this line\n";
    }
}

sub parse_xl ($self, $in) {
    while (<$in>) {
        if (/$at_c/) {
            return 'c';
        }
        elsif (/$at_xl/ || !/\S/ || m{\A\s*//}) {
            next;
        }
        else {
            $self->parse_directive($in, $_);
        }
    }
    return undef;
}


sub parse_top_level ($self, $in) {
    my $mode = 'c';
    while ($mode) {
        my $method = "parse_$mode";
        $mode = $self->$method($in);
    }
}


sub parse ($class, $arg, $in) {
    my $self = $class->new();
    eval { $self->parse_top_level($in); 1 } or do {
        my $error = $@ =~ s/\s+\z//r;
        die "$error in $arg line $..\n";
    };
    return $self;
}

1;
