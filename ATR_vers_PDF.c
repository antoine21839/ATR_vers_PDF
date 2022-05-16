#include "ATR_vers_PDF.h"

static void ecrire_debut(FILE *f){
    fprintf(f, "digraph arbre {\n");
    fprintf(f, "\tnode [shape=record, height=.1 ]\n");
    fprintf(f, "\tedge [tailclip=false, arrowtail = dot, dir=both];\n\n");
}

static void ecrire_arbre(FILE *f, ATR a){
    char etiquette[3];
    if(NULL == a)
        return;
    if('\0' == a->c)
        sprintf(etiquette, "\\0");
    else
        sprintf(etiquette, "%c", a->c);
    fprintf(f,"\tn%p [label=\"<fg> | <fc> | <valeur> %s | <fd> \"];\n", (void *)a, etiquette);
    if(NULL != a->fg){
        fprintf(f, "\tn%p:fg:c -> n%p:valeur;\n", (void *)a, (void *)a->fg);
        ecrire_arbre(f, a->fg);
    }
    if(NULL != a->fc){
        fprintf(f, "\tn%p:fc:c -> n%p:valeur;\n", (void *)a, (void *)a->fc);
        ecrire_arbre(f, a->fc);
    }
    if(NULL != a->fd){
        fprintf(f, "\tn%p:fd:c -> n%p:valeur;\n", (void *)a, (void *)a->fd);
        ecrire_arbre(f, a->fd);
    }
}

static void ecrire_fin(FILE *f){
    fprintf(f, "}\n");
}

static void dessine(FILE *f, ATR a) {
    ecrire_debut(f);
    ecrire_arbre(f, a);
    ecrire_fin(f);
}


int cree_pdf(ATR a, const char *pdf){
    FILE *out;
    char commande[TAILLE_MAX];

    assert(NULL != pdf);

    sprintf(commande, "dot -Tpdf -o %s", pdf);
    out = popen(commande, "w");
    if(NULL == out){
        fprintf(stderr, "Erreur lors de l'ouverture du pipe de sortie vers la commande \"%s\" !\n", commande);
        return 0;
    }
    dessine(out, a);
    pclose(out);
    return 1;
}
