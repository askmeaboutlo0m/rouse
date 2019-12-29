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
package Rouse::Svg::Reification;
use Moo;
use strictures 2;
use namespace::clean;
use experimental qw(signatures);
use List::Util qw(all);


has _buf => (
    is       => 'rw',
    default  => sub {
        my $buf = '';
        return \$buf;
    },
);

has _fh => (
    is        => 'rw',
    clearer   => 1,
    predicate => 1,
    default   => sub {
        open my $fh, '>>', shift->_buf or die "Can't open string buffer: $!\n";
        binmode $fh;
        return $fh;
    },
);

has count => (
    is      => 'rw',
    default => sub { 0 },
);

has [qw/x y/] => (
    is      => 'rw',
    default => sub { 0.0 },
);

has points => (
    is      => 'rw',
    default => sub { [] },
);

has matrix => (
    is        => 'rw',
    clearer   => 1,
    predicate => 1,
);


sub to_c_float ($n) {
    return ($n =~ /\./ ? $n : "$n.0") . 'f';
}

sub to_c_floats (@ns) {
    return map { to_c_float($_) } @ns;
}


sub _matrix_equal ($m, $n) {
    return @$m == @$n && all { $m->[$_] == $n->[$_] } 0 .. $#$m;
}

sub next_matrix ($self, $matrix) {
    if ($self->has_matrix) {
        my $prev = $self->matrix;
        if (defined $prev) {
            if (defined $matrix && _matrix_equal($prev, $matrix)) {
                return 0;
            }
            else {
                $self->matrix($matrix);
                return 1;
            }
        }
        else {
            if (defined $matrix) {
                $self->matrix($matrix);
                return 1;
            }
            else {
                return 0;
            }
        }
    }
    else {
        $self->matrix($matrix);
        return 1;
    }
}


sub clear_points ($self) {
    $self->points([]);
}


sub _point_equal ($p, $q) {
    return $p->[0] == $q->[0] && $p->[1] == $q->[1];
}

sub points_closed ($self) {
    my $points = $self->points;
    return @$points && _point_equal(@{$points}[0, -1]);
}


sub pos ($self, @args) {
    if (@args == 0) {
        return ($self->x, $self->y);
    }
    elsif (@args == 2) {
        $self->x($args[0]);
        $self->y($args[1]);
        push @{$self->points}, [@args];
        return @args;
    }
    else {
        die "Bad number of arguments to pos\n";
    }
}

sub shift ($self, $x, $y) {
    return ($self->x + $x, $self->y + $y);
}

sub shift_pos ($self, $x, $y) {
    return $self->pos($self->shift($x, $y));
}


sub add ($self, $n = 1) {
    $self->count($self->count + $n);
    return $self;
}


sub print ($self, @args) {
    $self->_fh->print(@args);
    return $self;
}

sub printf ($self, $fmt, @args) {
    $self->_fh->printf($fmt, @args);
    return $self;
}

sub pack ($self, $fmt, @args) {
    $self->print(pack $fmt, @args);
}

sub pack_floats ($self, @floats) {
    for my $float (map { "$_" } map { 0+$_ } @floats) {
        $self->pack('C', length "$float");
        $self->print($float);
    }
}

sub close ($self) {
    if ($self->_has_fh) {
        $self->_fh->close() or die "Can't close string buffer: $!\n";
        $self->_clear_fh();
    }
    return $self;
}

sub get_result ($self) {
    $self->close;
    return ${$self->_buf};
}


1;
