()  proc newline
    10 printchar
end

//"inc/reflect.inc" include
//"int/sort.inc" include


//string

(String:s String:t -> Z32) "_INTERNAL"  "zstrcmp" sys strcmp

(String:s Size32:count -> String$) "_INTERNAL"  "zstrndup" sys left

(String$:dest String:src Size32:count Size32:count -> String$) "_INTERNAL" "zstrcatsub" sys midcat


//string append
(String$:dest String:src -> String$) proc ++
    dest$$ src 0 -1 midcat
end

//string append indirect
(String$&:dest String:src) proc ++

    dest$$ src 0 -1 midcat dest=
end



(String:a String:b -> Bit) proc ==
    a b strcmp 0 == return
end

(String:a String:b -> Bit) proc >
    a b strcmp 1 == return
end

(String:a String:b -> Bit) proc <
    a b strcmp -1 == return
end



//inc/dec

(Z32&:a) proc ++
    a 1 + a =
end

(Z32&:a) proc --
    a 1 - a =
end
