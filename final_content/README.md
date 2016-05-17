<h1>Collisions sur signatures pdf</h1>

Ce dépôt présente des documents pdf différents mais pourtant signés de la même manière.

Le premier exemple est donné par le couple modele1.pdf / powned1.pdf.
powned1.pdf est en effet construit uniquement à partir de modele1.pdf et constitue donc une attaque directement applicable à n'importe quel document pdf signé vérifiant la condition que beaucoup de 0 soient placés dans le padding de la signature.
On pourra vérifier (après avoir véfirié le code du programme extract_and_check dans src/extract_and_check.cpp) que les 2 documents sont bien correctement signés (en tapant *make verify_pdf1* et *make verify_pdf2*).

Il est très important de comprendre la démarche :
dans ce cas, on a crée un document modele1.pdf qu'on a signé avec une identité convenable. On a par la suite repris le document crée pour le modifier (au niveau de l'objet signature) afin que le nouveau document produise un affichage différent. On peut vérifier que la signature reste inchangée alors que l'affichage est altéré : l'attaque a réussi.


Le second exemple, donné par le couple modele2.pdf / powned2.pdf, et plus une démonstration de faiblesse qu'une attaque.
On a construit dans ce cas les 2 documents ensemble, les 2 documents étant identiquement signés mais présentant un visuel complètement différent (ici le visuel n'est pas très différent sur la forme, seuls les textes sont modifiés, essentiellement parce que cela ne présente pas beaucoup d'intérêt d'afficher quelque chose de complètement différent).

On pourra vérifier les signature de ces documents en tapant *make verify_pdf3* et *make verify_pdf4*.
