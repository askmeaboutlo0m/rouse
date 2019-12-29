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
package Rouse::Svg::Path;
use Moo;
use strictures 2;
use namespace::clean;
use experimental qw(signatures);
use Parse::RecDescent;
use Rouse::Svg::Color;
use Rouse::Svg::Command::AbsM;
use Rouse::Svg::Command::RelC;
use Rouse::Svg::Command::RelL;
use Rouse::Svg::Command::RelM;
use Rouse::Svg::Command::Z;
use Rouse::Svg::Transform;


has [qw/fill transform commands/] => (
    is       => 'ro',
    required => 1,
);


my $grammar = q{
    <skip:qr/[,\s]*/>

    path: command(s?) eof { [map { @$_ } @{$item[1]}] }

    command: abs_m | rel_m | rel_l | rel_c | z | unknown_command

    abs_m: 'M' tup2(s) { [map { Rouse::Svg::Command::AbsM->new(@$_) } @{$item[2]}] }
    rel_m: 'm' tup2(s) { [map { Rouse::Svg::Command::RelM->new(@$_) } @{$item[2]}] }
    rel_l: 'l' tup2(s) { [map { Rouse::Svg::Command::RelL->new(@$_) } @{$item[2]}] }
    rel_c: 'c' tup6(s) { [map { Rouse::Svg::Command::RelC->new(@$_) } @{$item[2]}] }

    z: /[zZ]/ { [Rouse::Svg::Command::Z->new] }

    unknown_command: /[a-zA-Z]+/ {
        die "Unknown command '$item[1]' at line $thisline, column $thiscolumn\n";
    }

    tup2: number number                             { [@item[1 .. 2]] }
    tup6: number number number number number number { [@item[1 .. 6]] }

    number: /-?\d*\.?\d+/ { 0+$item[1] }

    eof: /\A\s*\z/
};

my $parser = do {
    local $::RD_ERRORS = 1;
    local $::RD_WARN   = 1;
    local $::RD_HINT   = 1;
    Parse::RecDescent->new($grammar)
} or die "Bad grammar: $grammar";


sub _parse_commands ($input) {
    my $paths = $parser->path($input);
    if (defined $paths) {
        return $paths;
    }
    else {
        die "Can't parse '$input'\n";
    }
}

sub _parse_path ($elem) {
    return (
        fill      => Rouse::Svg::Color->new($elem->att('fill') // '#000'),
        transform => Rouse::Svg::Transform->new($elem->att('transform')),
        commands  => _parse_commands($elem->att('d')),
    );
}

around BUILDARGS => sub ($orig, $class, $elem) {
    return $class->$orig(_parse_path($elem));
};


sub to_c ($self, $r) {
    $r->print("    nvgBeginPath(nvg);\n");
    $r->print('    nvgFillColor(nvg, ');
    $self->fill->to_c($r);
    $r->print(");\n");
    $self->transform->to_c($r);

    for my $command (@{$self->commands}) {
        $command->to_c($r);
    }

    $r->print("    nvgFill(nvg);\n");
}

sub to_rvg ($self, $r) {
    $r->add->print('a');
    $self->transform->to_rvg($r);
    $self->fill->to_rvg($r);

    for my $command (@{$self->commands}) {
        $command->to_rvg($r);
    }

    $r->add->print('f');
}


1;
