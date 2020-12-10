#include "../common.h"
#include "../csv lib/csv reader.c"
#include "./Arvore binaria.h"

#define NONE 0
#define CASOS 1
#define SEDENTARISMO 2
#define SONO 3
#define ALIMENTACAO 4
#define PSICOLOGICO 5

int treeOrdering = NONE;

int main() {
    FILE *file;
    CSV csv;
    BINARY_TREE tree;
    BINARY_TREE *busca;
    char optionUser[30];
    int choice;
    int ordenacao;
    int teveABusca = 0;

    file = fopen("amostra de dados.csv", "r");
    setRowsAndColumns(file, &csv);
    readCSV(file, &csv);
    printCSV(csv);
    printf("\n(Digite help para saber mais)\n");

    do {
        // Solicitação para o usuário
        printf(">>> ");
        setbuf(stdin, NULL);
        scanf("%[^\n]s", optionUser);
        
        // Ordenação
        if(strcmp(optionUser, "ordenate") == 0) {
            
            printf("Ordernar por...\n[ 1 ] Quantidade de casos\n[ 2 ] Nível de sedentarismo\n[ 3 ] Qualidade do sono\n[ 4 ] Qualidade da alimentação\n[ 5 ] Qualidade psicológica\n");
            printf("Escolha uma opção: ");
            scanf("%d", &choice);

            if(treeOrdering != choice) {
                generateTree(&tree, &csv, choice);
                treeOrdering = choice;
            }
            teveABusca = 0;
            
            printf("Tipo de ordenação...\n[ 1 ] Crescente\n[ 2 ] Decrescente\nEscolha a opção: ");
            scanf("%d", &ordenacao);
            if(ordenacao == 1) {
                printTitle(csv);
                ascendingOrder(tree);
            } else {
                printTitle(csv);
                descendingOrder(tree);
            }
        }
        else if(strcmp(optionUser, "generate csv file") == 0) {
            char nameOfFile[32];
            printf(">>> Nome do arquivo: ");
            setbuf(stdin, NULL);
            scanf("%31[^\n]s", nameOfFile);

            if(teveABusca) {
                if(generateFromTreeToCSVFile(busca, nameOfFile, 1)) {
                    printf("File created successfully!!!\n");
                }
            } else {
                if(treeOrdering > 0) {
                    if(generateFromTreeToCSVFile(&tree, nameOfFile, ordenacao)) {
                        printf("File created successfully!!!\n");
                    }
                } else {
                    if(generateFromCsvToCSVFile(csv, nameOfFile)) {
                        printf("File created successfully!!!\n");
                    }
                }
            }
        } else if(strcmp(optionUser, "search") == 0) {
            int quantidade;
            
            printf("Procurar com base em...\n[ 1 ] Quantidade de casos\n[ 2 ] Nível de sedentarismo\n[ 3 ] Qualidade do sono\n[ 4 ] Qualidade da alimentação\n[ 5 ] Qualidade psicológica\n");

            printf("Escolha uma opção: ");
            scanf("%d", &choice);

            if(treeOrdering != choice) {
                generateTree(&tree, &csv, choice);
                treeOrdering = choice;
            }
            printf("Quantidade: ");
            scanf("%d", &quantidade);
            
            printTitle(csv);
            busca = buscaNaArvore(&tree, quantidade, choice);
            teveABusca = 1;
            ascendingOrder(*busca);
        } else if(strcmp(optionUser, "help") == 0) {
            printf("\nFunções:\n- ordenate (faz a ordenação crescente ou decrescente dos dados a partir de um campo especificado)\n- generate csv file (gera arquivo csv a partir da última tabela amostrada no programa)\n- search (faz a busca de um determinado valor dentro dos dados a partir de um campo especificado)\n- exit (finaliza o programa)\n- help (amostra todas as funcionalidades do programa)\n\n");
        }
    } while(strcmp(optionUser, "exit"));
}

void centerText(char *text, int fieldWidth) {
    int padlen = (fieldWidth - strlen(text)) / 2;
    printf("%*s%s%*s\n", padlen, "", text, padlen, "");
}

void makeTree(BINARY_TREE *tree, VALUES values) {
    *tree = (BINARY_TREE) malloc (sizeof (NODE));
    if (!(*tree)) {
        printf("Erro! Nao existe memoria disponivel!");
        exit (1);
    }
    strcpy((*tree)->informations.estado, values.estado);

    (*tree)->informations.casos = values.casos;
    (*tree)->informations.saude.estadoPsicologico = values.saude.estadoPsicologico;
    (*tree)->informations.saude.qualidadeDaAlimentacao = values.saude.qualidadeDaAlimentacao;
    (*tree)->informations.saude.qualidadeDoSono = values.saude.qualidadeDoSono;
    (*tree)->informations.saude.sedentarismo = values.saude.sedentarismo;

    (*tree)->left = (*tree)->father = (*tree)->right = NULL;
}

void setLeft(BINARY_TREE tree, VALUES values) {
    tree->left = (BINARY_TREE) malloc (sizeof (NODE));
    if (!(tree->left)) {
        printf("Erro! Nao existe memoria disponivel!");
        exit (1);
    }
    strcpy(tree->left->informations.estado, values.estado);

    tree->left->informations.casos = values.casos;
    tree->left->informations.saude.estadoPsicologico = values.saude.estadoPsicologico;
    tree->left->informations.saude.qualidadeDaAlimentacao = values.saude.qualidadeDaAlimentacao;
    tree->left->informations.saude.qualidadeDoSono = values.saude.qualidadeDoSono;
    tree->left->informations.saude.sedentarismo = values.saude.sedentarismo;

    tree->left->left = tree->left->right = NULL;
    tree->left->father = tree;
}

void setRight(BINARY_TREE tree, VALUES values) {
    tree->right = (BINARY_TREE) malloc (sizeof (NODE));
    if (!(tree->right)) {
        printf("Erro! Nao existe memoria disponivel!");
        exit (1);
    }
    strcpy(tree->right->informations.estado, values.estado);

    tree->right->informations.casos = values.casos;
    tree->right->informations.saude.estadoPsicologico = values.saude.estadoPsicologico;
    tree->right->informations.saude.qualidadeDaAlimentacao = values.saude.qualidadeDaAlimentacao;
    tree->right->informations.saude.qualidadeDoSono = values.saude.qualidadeDoSono;
    tree->right->informations.saude.sedentarismo = values.saude.sedentarismo;

    tree->right->right = tree->right->left = NULL;
    tree->right->father = tree;
}

VALUES valueOfNode(BINARY_TREE tree) {
    if(tree == NULL) {
        printf("\nThe tree is empty!\n");
        exit(2);
    }
    return tree->informations;
}

BINARY_TREE left(BINARY_TREE tree) {
    return tree->left;
}

BINARY_TREE right(BINARY_TREE tree) {
    return tree->right;
}

BINARY_TREE father(BINARY_TREE tree) {
    return tree->father;
}

BINARY_TREE brother(BINARY_TREE tree) {
    if (father(tree)){
        if (isLeft(tree)){
            return right(father(tree));
        } else {
            return left(father(tree));
        }
    } else {
        return NULL;
    }
}

int isLeft(BINARY_TREE tree) {
    NODE *q = father(tree);
    if (!q)
        return (0);
    if (left(q) == tree)
        return (1);
    return (0);
}

int isRight(BINARY_TREE tree) {
    if (father(tree))
        return (!isLeft(tree));
    return (0);
}

int putOnTheSide(VALUES values1, VALUES values2, int organizate) {
    int aux;
    switch(organizate) {
        case 1: 
            if(values1.casos < values2.casos) {
                aux = -1;
            } else if(values1.casos > values2.casos) {
                aux = 1;
            } else {
                aux = 0;
            }
            break;
        case 2:
            if(values1.saude.sedentarismo < values2.saude.sedentarismo) {
                aux = -1;
            } else if(values1.saude.sedentarismo > values2.saude.sedentarismo) {
                aux = 1;
            } else {
                aux = 0;
            }
            break;
        case 3:
            if(values1.saude.qualidadeDoSono < values2.saude.qualidadeDoSono) {
                aux = -1;
            } else if(values1.saude.qualidadeDoSono > values2.saude.qualidadeDoSono) {
                aux = 1;
            } else {
                aux = 0;
            }
            break;
        case 4:
            if(values1.saude.qualidadeDaAlimentacao < values2.saude.qualidadeDaAlimentacao) {
                aux = -1;
            } else if(values1.saude.qualidadeDaAlimentacao > values2.saude.qualidadeDaAlimentacao) {
                aux = 1;
            } else {
                aux = 0;
            }
            break;
        case 5:
            if(values1.saude.estadoPsicologico < values2.saude.estadoPsicologico) {
                aux = -1;
            } else if(values1.saude.estadoPsicologico > values2.saude.estadoPsicologico) {
                aux = 1;
            } else {
                aux = 0;
            }
            break;
    }
    return aux;
}

void insertElement(BINARY_TREE *tree, VALUES values, int organizate) {
    if (!(*tree))
        makeTree(tree, values);
    else {
        BINARY_TREE father = (*tree);
        do {
            if(putOnTheSide(values, valueOfNode(father), organizate) == -1) {
                if(father->left)
                    father = father->left;
                else {
                    setLeft(father, values);
                    break;
                }
            } else {
                if(father->right)
                    father = father->right;
                else {
                    setRight(father, values);
                    break;
                }
            }
        } while(TRUE);
    }
}

void removalByFusion(BINARY_TREE *tree) {
    if (*tree) {
        BINARY_TREE aux = *tree;
        if (!((*tree)->right))
            *tree = (*tree)->left;
        else
            if ((*tree)->left == NULL)
                *tree = (*tree)->right;
            else {
                aux = (*tree)->left;
                while (aux->right)
                aux = aux->right;
                aux->right = (*tree)->right;
                aux->right ->father= aux;
                aux = *tree;
                *tree = (*tree)->left;
            }
        free(aux);
    }
}

void removalByCopy(BINARY_TREE *tree) {
    if (*tree) {
        BINARY_TREE aux = *tree;
        if ((*tree)->right == NULL) {
            *tree = (*tree)->left;
            (*tree)->father=NULL;
        } else
            if ((*tree)->left == NULL) {
                *tree = (*tree)->right;
                (*tree)->father=NULL;
            } else {
                aux = (*tree)->right;
                while (aux->left!=NULL)
                    aux = aux->left;

                strcpy((*tree)->informations.estado, aux->informations.estado);

                (*tree)->informations.casos = aux->informations.casos;
                (*tree)->informations.saude.estadoPsicologico = aux->informations.saude.estadoPsicologico;
                (*tree)->informations.saude.qualidadeDaAlimentacao = aux->informations.saude.qualidadeDaAlimentacao;
                (*tree)->informations.saude.qualidadeDoSono = aux->informations.saude.qualidadeDoSono;
                (*tree)->informations.saude.sedentarismo = aux->informations.saude.sedentarismo;

                if (aux->father == *tree) {
                    aux->father->right = aux->right;
                    aux->father->right->father = aux->father;
                } else {
                    aux->father->left = aux->right;
                    aux->father->left->father = aux->father;
                }
            }
        free(aux);
    }
}

void preOrderRoute(BINARY_TREE tree) {
    if (tree) {
        printf("| ");
        printf("%*s%*s| ", -12, valueOfNode(tree).estado, 4, "");
        printf("%-16d| ", valueOfNode(tree).casos);
        printf("%8d%-8c| ", valueOfNode(tree).saude.sedentarismo, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDoSono, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDaAlimentacao, '%');
        printf("%8d%-8c| \n", valueOfNode(tree).saude.estadoPsicologico, '%');
        preOrderRoute(left(tree));
        preOrderRoute(right(tree));
    }
}

void postOrderRoute(BINARY_TREE tree) {
    if (tree) {
        postOrderRoute(left(tree));
        postOrderRoute(right(tree));
        printf("| ");
        printf("%*s%*s| ", -12, valueOfNode(tree).estado, 4, "");
        printf("%-16d| ", valueOfNode(tree).casos);
        printf("%8d%-8c| ", valueOfNode(tree).saude.sedentarismo, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDoSono, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDaAlimentacao, '%');
        printf("%8d%-8c| \n", valueOfNode(tree).saude.estadoPsicologico, '%');
    }
}

void ascendingOrder(BINARY_TREE tree) {
    if (tree) {
        ascendingOrder(left(tree));
        printf("| ");
        printf("%*s%*s| ", -12, valueOfNode(tree).estado, 4, "");
        printf("%-16d| ", valueOfNode(tree).casos);
        printf("%8d%-8c| ", valueOfNode(tree).saude.sedentarismo, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDoSono, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDaAlimentacao, '%');
        printf("%8d%-8c| \n", valueOfNode(tree).saude.estadoPsicologico, '%');
        ascendingOrder(right(tree));
    }
}

void descendingOrder(BINARY_TREE tree) {
    if(tree) {
        descendingOrder(right(tree));
        printf("| ");
        printf("%*s%*s| ", -12, valueOfNode(tree).estado, 4, "");
        printf("%-16d| ", valueOfNode(tree).casos);
        printf("%8d%-8c| ", valueOfNode(tree).saude.sedentarismo, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDoSono, '%');
        printf("%8d%-8c| ", valueOfNode(tree).saude.qualidadeDaAlimentacao, '%');
        printf("%8d%-8c| \n", valueOfNode(tree).saude.estadoPsicologico, '%');
        descendingOrder(left(tree));
    }
}

void generateTree(BINARY_TREE *tree, CSV *csv, int organizate) {
    int line = 2;
    VALUES *values;
    makeTree(tree, (*fromCSVToValue(csv, line)));
    for(line++; line <= csv->row; line++) {
        values = fromCSVToValue(csv, line);
        insertElement(tree, *values, organizate);
    }
}

VALUES *fromCSVToValue(CSV *csv, int line) {
    VALUES *values = (VALUES *)malloc(sizeof(VALUES));

    strcpy(values->estado, csv->array[line - 1][0]);
    values->casos = atoi(csv->array[line - 1][1]);
    values->saude.sedentarismo = atoi(csv->array[line - 1][2]);
    values->saude.qualidadeDoSono = atoi(csv->array[line - 1][3]);
    values->saude.qualidadeDaAlimentacao = atoi(csv->array[line - 1][4]);
    values->saude.estadoPsicologico = atoi(csv->array[line - 1][5]);
    return values;
}

void printVALUES(VALUES values) {
    printf("\n%s = %s%c", "Estado", values.estado, '\n');
    printf("%s = %d%c", "Casos", values.casos, '\n');
    printf("%s = %d%c", "Sedentarismo", values.saude.sedentarismo, '\n');
    printf("%s = %d%c", "Sono", values.saude.qualidadeDoSono, '\n');
    printf("%s = %d%c", "Alimentação", values.saude.qualidadeDaAlimentacao, '\n');
    printf("%s = %d%s", "Estado psicológico", values.saude.estadoPsicologico, "\n\n");
}

BINARY_TREE *buscaNaArvore(BINARY_TREE *tree, int quantidade, int escolha) {
    BINARY_TREE *busca = (BINARY_TREE *)malloc(sizeof(BINARY_TREE));
    BINARY_TREE father = (*tree);
    VALUES values;
    
    switch(escolha) {
        case CASOS:
            values.casos = quantidade;
            break;
        case SEDENTARISMO:
            values.saude.sedentarismo = quantidade;
            break;
        case SONO:
            values.saude.qualidadeDoSono = quantidade;
            break;
        case ALIMENTACAO:
            values.saude.qualidadeDaAlimentacao = quantidade;
            break;
        case PSICOLOGICO:
            values.saude.estadoPsicologico = quantidade;
            break;
    }
    do {
        int aux = putOnTheSide(values, valueOfNode(father), escolha);
        if(aux == -1) {
            if(father->left)
                father = father->left;
            else {
                break;
            }
        }
        else {
            if(aux == 0) {
                if(busca) {
                    insertElement(busca, valueOfNode(father), escolha);
                } else {
                    makeTree(busca, valueOfNode(father));
                }
            }
            if(father->right)
                father = father->right;
            else {
                break;
            }
        }
    } while(TRUE);
    return busca;
}

FILE *generateFromTreeToCSVFile(BINARY_TREE *tree, char *nameOfFile, int order){
    if(strlen(nameOfFile) > 30){
        puts("The file name exceed 30 characters!");
        return NULL;
    }
 
    char name[35];
    strcpy(name, nameOfFile);
    strcat(name, ".csv");
 
    FILE *aux;
    aux = fopen(name, "w");
    
    printInFile(*tree, aux, order);
    
    fclose(aux);
    return aux;
}
 
void printInFile(BINARY_TREE tree, FILE *file, int order) {
    if(order == 1){
        if(tree){
            printInFile(left(tree), file, order);
            fprintf(file, "%s,", valueOfNode(tree).estado);
            fprintf(file, "%d,", valueOfNode(tree).casos);
            fprintf(file, "%d,", valueOfNode(tree).saude.sedentarismo);
            fprintf(file, "%d,", valueOfNode(tree).saude.qualidadeDoSono);
            fprintf(file, "%d,", valueOfNode(tree).saude.qualidadeDaAlimentacao);
            fprintf(file, "%d\n", valueOfNode(tree).saude.estadoPsicologico);
            printInFile(right(tree), file, order);
        }
    } else {
        if(tree){
            printInFile(right(tree), file, order);
            fprintf(file, "%s,", valueOfNode(tree).estado);
            fprintf(file, "%d,", valueOfNode(tree).casos);
            fprintf(file, "%d,", valueOfNode(tree).saude.sedentarismo);
            fprintf(file, "%d,", valueOfNode(tree).saude.qualidadeDoSono);
            fprintf(file, "%d,", valueOfNode(tree).saude.qualidadeDaAlimentacao);
            fprintf(file, "%d\n", valueOfNode(tree).saude.estadoPsicologico);
            printInFile(left(tree), file, order);
        }
    }
}
 
FILE *generateFromCsvToCSVFile(CSV csv, char *nameOfFile){
    if(strlen(nameOfFile) > 30){
        puts("The file name exceed 30 characters!");
        return NULL;
    }
 
    char name[35];
    strcpy(name, nameOfFile);
    strcat(name, ".csv");
 
    FILE *aux;
    aux = fopen(name, "w");
 
    for(int i = 0; i < csv.row; i++) {
        for(int j = 0; j < csv.column; j++) {
            j == csv.column - 1 ? fprintf(aux, "%s\n", csv.array[i][j]) : fprintf(aux, "%s,", csv.array[i][j]);
        }
    }
 
    fclose(aux);
    return aux;
}

void clearBuffer(){
    scanf("%*[^\n]s");
    scanf("%*c");
}