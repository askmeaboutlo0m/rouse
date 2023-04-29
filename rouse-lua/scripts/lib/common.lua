-- Copyright (c) 2019, 2020 askmeaboutloom
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


function slurp(filename)
    local fh <close> = assert(io.open(filename, "rb"))
    return fh:read("*a")
end

function slurplines(filename)
    local lines = {}
    for line in io.lines(filename) do
        table.insert(lines, line)
    end
    return lines
end


function dofile_env(filename, env)
    local result, err = loadfile(filename, "bt", env)
    if result then
        return result()
    else
        error(err)
    end
end


redofile_caching = false

redofile = (function ()
    local cache = {}

    local function dofile_maybe_env(filename, env)
        if env then
            return dofile_env(filename, env)
        else
            return dofile(filename)
        end
    end

    return function (filename, env)
        if redofile_caching then
            local cached = cache[filename]
            if cached then
                return cached
            else
                local value     = dofile_maybe_env(filename, env)
                cache[filename] = value
                return value
            end
        else
            return dofile_maybe_env(filename, env)
        end
    end
end)()


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

function is_thread(x)
    return type(x) == "thread"
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

function table.imerge(...)
    local merged = {}
    for i, arg in ipairs {...} do
        for i, value in ipairs(arg) do
        table.insert(merged, value)
        end
    end
    return merged
end

function table.rollindex(t)
    return R.rand(1, #t)
end

function table.roll(t)
    return t[table.rollindex(t)]
end

function table.pick(t)
    return table.remove(t, table.rollindex(t))
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


function string.split(s, pattern)
    local pieces = {}
    local index  = 0

    while index do
        local from, to = string.find(s, pattern, index)
        if from then
            table.insert(pieces, string.sub(s, index, from - 1))
            index = to + 1
        else
            table.insert(pieces, string.sub(s, index, #s))
            index = nil
        end
    end

    return pieces
end


function math.clamp(value, min, max)
    if value < min then
        return min
    elseif value > max then
        return max
    else
        return value
    end
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
        al = {
            enabled = args.openal and true or false,
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
