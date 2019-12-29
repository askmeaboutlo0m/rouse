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
package Rouse::Svg::Transform;
use Moo;
use strictures 2;
use namespace::clean;
use experimental qw(signatures);


has matrix => (
    is       => 'ro',
    required => 1,
);


sub _parse_matrix ($input) {
    return (matrix => undef) unless defined $input;

    my $number = qr/-?\d*\.?\d+/;
    my $sep    = qr/[,\s]+/;
    my $rx     = qr/
        \A \s* matrix \s* \( \s*
            ($number) $sep
            ($number) $sep
            ($number) $sep
            ($number) $sep
            ($number) $sep
            ($number)
        \s* \) \s*\z
    /x;

    if ($input =~ $rx) {
        return (matrix => [map { 0+$_ } $1, $2, $3, $4, $5, $6]);
    }
    else {
        die "Can't parse transform '$input'\n";
    }
}

around BUILDARGS => sub ($orig, $class, $input) {
    return $class->$orig(_parse_matrix($input));
};


sub equal_to ($self, $other) {
    my $m = $self ->matrix || [];
    my $n = $other->matrix || [];
    return @$m == @$n && all { $m->[$_] == $n->[$_] } 0 .. $#$m;
}


sub to_c ($self, $r) {
    my $matrix = $self->matrix;
    if ($r->next_matrix($matrix)) {
        $r->print("    nvgResetTransform(nvg);\n");
        if (defined $matrix) {
            $r->printf("    nvgTransform(nvg, %s, %s, %s, %s, %s, %s);\n",
                       Rouse::Svg::Reification::to_c_floats(@$matrix));
        }
    }
}

sub to_rvg ($self, $r) {
    my $matrix = $self->matrix;
    if ($r->next_matrix($matrix)) {
        $matrix //= [1, 0, 0, 1, 0, 0];
        $r->add->print('t');
        $r->pack_floats(@$matrix);
    }
}


1;
