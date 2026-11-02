func	vecAdd		
=	0		i
label			L0
if<	i	n	L1
goto			L2
label			L1
*	i	4	t1
+	c	t1	t2
*	i	4	t3
+	a	t3	t4
*	i	4	t5
+	b	t5	t6
=*	t4		t7
=*	t6		t8
+	t7	t8	t9
*=	t2	t9	
+	i	1	t0
=	t0		i
goto			L0
label			L2
return	0		
endfunc			
