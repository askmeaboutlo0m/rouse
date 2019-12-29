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
package Rouse::Svg::Color;
use Moo;
use strictures 2;
use namespace::clean;
use experimental qw(signatures);


has [qw/r g b a/] => (
    is       => 'ro',
    required => 1,
);


sub _from_hex ($r, $g, $b, $a) {
    return (r => hex $r, g => hex $g, b => hex $b, a => hex $a);
}

sub _parse_color ($color) {
    $color =~ s/\A\s+|\s+\z//g;
    if ($color =~ /\A#([0-9a-f])([0-9a-f])([0-9a-f])\z/) {
        return _from_hex("$1$1", "$2$2", "$3$3", 'ff');
    }
    elsif ($color =~ /\A#([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})\z/) {
        return _from_hex($1, $2, $3, 'ff');
    }
    else {
        die "Can't parse color: '$color'\n";
    }
}

around BUILDARGS => sub ($orig, $class, $color) {
    return $class->$orig(_parse_color($color));
};


sub to_c ($self, $r) {
    $r->printf("nvgRGBA(%d, %d, %d, %d)", $self->r, $self->g, $self->b, $self->a);
}

sub to_rvg ($self, $r) {
    $r->add->print('c')->pack('CCCC', $self->r, $self->g, $self->b, $self->a);
}


1;
