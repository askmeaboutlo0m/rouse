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
package Rouse::Svg;
use Moo;
use strictures 2;
use namespace::clean;
use experimental qw(signatures);
use XML::Twig;
use Rouse::Svg::Path;
use Rouse::Svg::Reification;


has [qw/width height paths/] => (
    is       => 'ro',
    required => 1,
);


sub _on_path ($elem) {
    my $name = lc $elem->gi;
    if ($name eq 'path') {
        return $elem;
    }
    elsif ($name eq 'g') {
        return _all_paths($elem);
    }
    else {
        warn 'Unknown element: ', $elem->sprint, "\n";
        return ();
    }
}

sub _all_paths ($elem) {
    return map { _on_path($_) } $elem->children('#ELT');
}

sub _parse_svg ($twig) {
    my $root = $twig->root;
    return (
        width  => $root->att('width'),
        height => $root->att('height'),
        paths  => [map { Rouse::Svg::Path->new($_) } _all_paths($root)],
    );
}

around BUILDARGS => sub ($orig, $class, $twig) {
    return $class->$orig(_parse_svg($twig));
};


sub from_file ($class, $path) {
    my $twig = XML::Twig->new;
    $twig->parsefile($path);
    return $class->new($twig);
}


sub to_c ($self, $name) {
    my $r = Rouse::Svg::Reification->new;
    $r->print("static void $name(NVGcontext *nvg)\n{\n");

    for my $path (@{$self->paths}) {
        $path->to_c($r);
    }

    $r->print("}\n");
    return $r->get_result;
}

sub to_rvg ($self) {
    my $r = Rouse::Svg::Reification->new;

    for my $path (@{$self->paths}) {
        $path->to_rvg($r);
    }

    my $packed = pack 'SSS', $self->width, $self->height, $r->count;
    return "rvector1\0$packed" . $r->get_result;
}


1;
