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
package Rouse::Svg::Command::RelC;
use Moo;
use strictures 2;
use namespace::clean;
use experimental qw(signatures);


extends 'Rouse::Svg::Command';

has [qw/dx1 dy1 dx2 dy2 dx dy/] => (
    is       => 'ro',
    required => 1,
);


around BUILDARGS => sub ($orig, $class, $dx1, $dy1, $dx2, $dy2, $dx, $dy) {
    return $class->$orig(
        dx1 => $dx1, dy1 => $dy1,
        dx2 => $dx2, dy2 => $dy2,
        dx  => $dx,  dy  => $dy,
    );
};


sub to_c ($self, $r) {
    my ($x1, $y1) = $r->shift($self->dx1, $self->dy1);
    my ($x2, $y2) = $r->shift($self->dx2, $self->dy2);
    my ($x , $y ) = $r->shift_pos($self->dx, $self->dy);
    $r->printf("    nvgBezierTo(nvg, %s, %s, %s, %s, %s, %s);\n",
               $self->_to_c_floats($x1, $y1, $x2, $y2, $x, $y));
}

sub to_rvg ($self, $r) {
    my ($x1, $y1) = $r->shift($self->dx1, $self->dy1);
    my ($x2, $y2) = $r->shift($self->dx2, $self->dy2);
    my ($x , $y ) = $r->shift_pos($self->dx, $self->dy);
    $r->add->print('C')->pack_floats($x1, $y1, $x2, $y2, $x, $y);
}


1;
