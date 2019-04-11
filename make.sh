CC="c++ -o 9 "
rm -f *.o
for f in `ls *[.]cc |grep -v portfwd.cc|grep -v lex.yy`; do t=`echo $f |sed 's/.cc$//g'`; $CC -o $t.o -c $f   -DPORTFWD_CONF=\"\" ; done
for f in `ls *[.]c| grep -v lex.yy`; do t=`echo $f |sed 's/[.]c$//g'`; $CC -o $t.o -c $f   -DPORTFWD_CONF=\"\" ; done

$CC -o portfwdXOR portfwd.cc *.o -DPORTFWD_CONF=\"\" 
