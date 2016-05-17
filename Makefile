VERIF_PROG = ./extract_and_check
INSERT_PROG = ./insert_sig
SRC_VERIF = src/extract_and_check.cpp
SRC_INSERT = src/insert_sig.cpp

all: $(VERIF_PROG) $(INSERT_PROG)

$(VERIF_PROG):
	g++ $(SRC_VERIF) -o $(VERIF_PROG)

$(INSERT_PROG):
	g++ $(SRC_INSERT) -o $(INSERT_PROG)

verify_pdf1:
	$(VERIF_PROG) modele1.pdf CA/ca.pem

verify_pdf2:
	$(VERIF_PROG) powned1.pdf CA/ca.pem

verify_pdf3:
	$(VERIF_PROG) modele2.pdf CA/ca.pem

verify_pdf4:
	$(VERIF_PROG) powned2.pdf CA/ca.pem

clean:
	rm -f $(VERIF_PROG)
	rm -f $(INSERT_PROG)
	rm -rf tmp