
.PHONY: all clean gen_drift.sh genrules_softdrift_left.sh genrules_softdrift_right.sh genrules_softdrift.sh

all: geninput gen_outer gen_left gen_left_soft gen_right gen_right_soft gen_drift.sh genrules_softdrift_left.sh genrules_softdrift_right.sh genrules_softdrift.sh 

clean:
	-rm geninput gen_outer gen_left gen_left_soft gen_right gen_right_soft

geninput: geninput.c

gen_outer: gen_outer.c

gen_left: gen_left.c

gen_left_soft: gen_left_soft.c

gen_right: gen_right.c

gen_right_soft: gen_right_soft.c

gen_drift.sh: 
	chmod +x gen_drift.sh

genrules_softdrift_left.sh:
	chmod +x genrules_softdrift_left.sh

genrules_softdrift_right.sh:
	chmod +x genrules_softdrift_right.sh

genrules_softdrift.sh:
	chmod +x genrules_softdrift.sh 
