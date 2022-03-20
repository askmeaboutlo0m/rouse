-- Copyright (c) 2022 askmeaboutloom
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.
-------------------------------------------------------------------------------
-- The mapping from phonemes to mouth shapes is vaguely based on Rhubarb Lip
-- Sync, which does something much more sophisticated, but was nonetheless the
-- basis for this file. Rhubarb Lip Sync is licensed under the MIT license,
-- which is reproduced here:
--
-- Rhubarb Lip Sync <https://github.com/DanielSWolf/rhubarb-lip-sync> is
-- released under the MIT License (MIT).
--
-- Copyright (c) 2015-2016 Daniel Wolf
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to
-- deal in the Software without restriction, including without limitation the
-- rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
-- sell copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
-- FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
-- IN THE SOFTWARE.

local LipSync <const> = {}

LipSync.phones_to_mouth_shapes = {
    ["SIL"  ] = "X",
    ["AO"   ] = "E",
    ["AA"   ] = "D",
    ["IY"   ] = "B",
    ["UW"   ] = "F",
    ["EH"   ] = "C",
    ["IH"   ] = "B",
    ["UH"   ] = "F",
    ["AH"   ] = "C",
    ["AE"   ] = "C",
    ["EY"   ] = "C",
    ["AY"   ] = "B",
    ["OW"   ] = "F",
    ["AW"   ] = "E",
    ["OY"   ] = "B",
    ["ER"   ] = "E",
    ["P"    ] = "A",
    ["B"    ] = "A",
    ["T"    ] = "B",
    ["D"    ] = "B",
    ["K"    ] = "B",
    ["G"    ] = "B",
    ["CH"   ] = "B",
    ["JH"   ] = "B",
    ["F"    ] = "G",
    ["V"    ] = "G",
    ["TH"   ] = "F",
    ["DH"   ] = "F",
    ["S"    ] = "B",
    ["Z"    ] = "B",
    ["SH"   ] = "F",
    ["ZH"   ] = "B",
    ["HH"   ] = "A",
    ["M"    ] = "A",
    ["N"    ] = "B",
    ["NG"   ] = "B",
    ["L"    ] = "H",
    ["R"    ] = "E",
    ["Y"    ] = "C",
    ["W"    ] = "F",
    ["+NSN+"] = "C",
    ["+SPN+"] = "F",
}

function LipSync.phone_to_mouth_shape(cls, phone)
    return phone and cls.phones_to_mouth_shapes[phone] or "X"
end

return LipSync
