section sort


   (Z32&:a Z32&:b ->Z32)& proc Z32compare

        a b == if
            0 return
        end

        a b < if
            -1 return
        end


        1 return

    end

    (Z32&:a Z32&:b ->Z32)& proc Z32compareR

        a b == if
            0 return
        end

        a b < if
            1 return
        end


        -1 return

    end


   ( [Z32]:arr Z32:reverse ) proc sort

        reverse if
            arr arr.count arr.size1 Z32compareR stdlib.qsort
        else
            arr arr.count arr.size1 Z32compare stdlib.qsort
        end
   end


end
