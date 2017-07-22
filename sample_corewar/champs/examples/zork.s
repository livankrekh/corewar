.name "zork"
.comment "I'M ALIIIIVE"

l2:		sti r19, %:live, %1

live:	live %1
		zjmp %:live
