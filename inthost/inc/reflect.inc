    //instructions
    (Any.Instruction:inst -> Z32 )                  "_INTERNAL" "instruction_opcode"    sys .opcode
    (Any.Instruction:inst -> String )               "_INTERNAL" "instruction_opname"    sys .opname
    (Any.Instruction:inst -> [Any.Instruction] )    "_INTERNAL" "instruction_args"      sys .args
    (Any.Instruction:inst -> Type )  "_INTERNAL"    "instruction_result_type"           sys .returnType

    //valTypeOf gives the Type of the constant value the instruction contains (is the constant a String, Integer, type, etc?)
    //valType gives the value the instruction contains, for when valTypeOf==Type   (the instruction carries a 'typeT*' as a constant)
    (Any.Instruction:inst -> Type )  "_INTERNAL"    "instruction_val_typeof"            sys .valTypeOf
    (Any.Instruction:inst -> String )  "_INTERNAL"  "instruction_val_pointer"           sys .valString
    (Any.Instruction:inst -> Z32 )  "_INTERNAL"     "instruction_val_z32"               sys .valZ32
    (Any.Instruction:inst -> String )  "_INTERNAL"  "instruction_val_pointer"           sys .valType
    (Any.Instruction:inst -> Word )  "_INTERNAL"    "instruction_val_pointer"           sys .valWord

    //If the word is to a C function, return that function pointer
    (Word:w -> Any* )  "_INTERNAL" "word_cfunc" sys .cfunc
    //todo: get the instruction array for a word, iterate its variables

    //types
    //Gives the name of a type
    //todo: get struct field members, etc
    (Type:t -> String )  "_INTERNAL" "type_keystr" sys .key


