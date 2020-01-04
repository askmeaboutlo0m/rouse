-- Copyright (c) 2019 askmeaboutloom
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in
-- all copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.

function class(base)
    local c   = {}
    c.__index = c

    if base then
        setmetatable(c, base)
        c.super = base
    end

    c.new = function (...)
        local o = setmetatable({}, c)
        o:init(...)
        return o
    end

    return c
end


function slurplines(filename)
    local lines = {}
    for line in io.lines(filename) do
        table.insert(lines, line)
    end
    return lines
end

function errorf(formatstring, ...)
    error(string.format(formatstring, ...))
end


function is_function(x)
    return type(x) == "function"
end

function is_integer(x)
    return is_number(x) and math.tointeger(x)
end

function is_number(x)
    return type(x) == "number"
end

function is_string(x)
    return type(x) == "string"
end

function is_userdata(x)
    return type(x) == "userdata"
end


function table.merge(...)
    local merged = {}
    for i, arg in ipairs {...} do
        for key, value in pairs(arg) do
            merged[key] = value
        end
    end
    return merged
end


function string.ltrim(s)
    return string.gsub(s, "^%s+", "")
end

function string.rtrim(s)
    return string.gsub(s, "%s+$", "")
end

function string.trim(s)
    return string.ltrim(string.rtrim(s))
end


function simple_main_args(args)
    local msaa      = args.msaa
    local have_msaa = msaa and msaa > 1
    return {
        window = {
            title  = args.title,
            width  = args.width,
            height = args.height,
        },
        gl = {
            red_size           = 8,
            green_size         = 8,
            blue_size          = 8,
            alpha_size         = 0,
            depth_size         = 0,
            stencil_size       = 8,
            multisamplebuffers = have_msaa and 1 or 0,
            multisamplesamples = have_msaa and msaa or nil,
        },
        on_scene = args.on_scene,
    }
end


setmetatable(_G, {
    __index = function (self, key)
        errorf("Reference to undeclared global '%s'", key)
    end,
    __newindex = function (self, key, value)
        errorf("Thwarted attempt to declare global '%s'", key)
    end,
})
