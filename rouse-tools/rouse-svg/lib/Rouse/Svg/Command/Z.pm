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
package Rouse::Svg::Command::Z;
use Moo;
use strictures 2;
use namespace::clean;
use experimental qw(signatures);
use List::Util qw(sum0);


extends 'Rouse::Svg::Command';


sub _get_edge ($points, $i) {
    my $src = $points->[$i];
    my $dst = $i < $#$points ? $points->[$i + 1] : $points->[0];
    return [$src, $dst];
}

sub _calc_edge_diff ($p, $q) {
    return ($q->[0] - $p->[0]) * ($q->[1] + $p->[1]);
}

sub _calc_edge_sum ($points) {
    return sum0 map { _calc_edge_diff(@$_) }
                map { _get_edge($points, $_) } 0 .. $#$points;
}

sub _get_path_winding ($r) {
    my $points = $r->points;
    if (@$points) {
        return _calc_edge_sum($points) <=> 0;
    }
    return 0;
}

sub to_c ($self, $r) {
    if (!$r->points_closed) {
        $r->print("    nvgClosePath(nvg);\n");
    }

    my $winding = _get_path_winding($r);
    if ($winding < 0) {
        $r->print("    nvgPathWinding(nvg, NVG_CCW);\n");
    }
    elsif ($winding > 0) {
        $r->print("    nvgPathWinding(nvg, NVG_CW);\n");
    }
    $r->clear_points;
}

sub to_rvg ($self, $r) {
    if (!$r->points_closed) {
        my ($x, $y) = @{$r->points->[0]};
        $r->add->print('L')->pack_floats($x, $y);
    }

    my $winding = _get_path_winding($r) > 0 ? 0 : 1;
    $r->add->print('w')->pack('C', $winding);
    $r->clear_points;
}


1;
