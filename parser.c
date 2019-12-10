#include <stdio.h> 
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

int Fsize=50; /* max string length of formulas*/
int inputs=10;
int i;
int j;

struct tree
{
  char *formula;
  struct tree *left;
  struct tree *right;
  struct tree *parent;
};

//This is a helper method used in partone and parttwo 
//This methods returns a new character pointer that contains the characters within the range provided
char *getCharacterInRange(char *inputChar, int start, int end){
  int final = end - start, newIndex = 0, i;
  char *newCharPointer = (char*)malloc(sizeof(char) * final);
  for(i = start; i < end ; i++){
      newCharPointer[newIndex] = inputChar[i];
      newIndex ++;
  }
  newCharPointer[newIndex] = '\0';
  return newCharPointer;
}

char *partone(char *inputChar){
  char *tmpStore = (char*)malloc(sizeof(char) * strlen(inputChar));
  int index = 0, brackets = 0, count = 0;
  while(inputChar[index+1] != '\0'){
    char current = inputChar[index];
    if (current == '(') {
        brackets ++;
    }
    else if (current == ')'){
        brackets --;
    }
    else if ((current == '^' || current == 'v' || current == '>') && brackets == 1){
        return getCharacterInRange(inputChar,1,index);
    }
    index ++;
  }
}

char *parttwo(char *inputChar){
  int length = strlen(inputChar), index = 0, brackets = 0, count = 0;
  char *tmpStore = (char*)malloc(sizeof(char) * length);
  while(inputChar[index+1] != '\0'){
    char current = inputChar[index];
    if (current == '(') {
        brackets ++;
    }
    else if (current == ')'){
        brackets --;
    }
    else if ((current == '^' || current == 'v' || current == '>') && brackets == 1){
        return getCharacterInRange(inputChar,index+1,length-1);
    }
    index ++;
  }
}

char *returnTail(char *inputChar) {
  // oldIndex is initialised to 1 as we want to skip the first character 
  int oldIndex = 1, newIndex = 0, length = strlen(inputChar);
  char *returnPointer = (char*) malloc(length * sizeof(char));
  while(inputChar[oldIndex] != '\0'){
      returnPointer[newIndex] = inputChar[oldIndex];
      oldIndex ++;
      newIndex ++;
  }
  returnPointer[newIndex] = '\0';
  return returnPointer;
}

int compareBracketAndConnectives(char *inputChar){
  int index = 0, brackets = 0, connectives  = 0;
  while(inputChar[index] != '\0'){
    char current = inputChar[index];
    if (current == '(' || current == ')'){
      brackets ++;
      index ++;
    }
    else if (current == '>' || current == '^' || current == 'v'){
      connectives ++;
      index ++;
    }
    else {index ++;}
  }
  connectives = connectives * 2;
  if(connectives == brackets){ return(1); }
  else{ return(0);}
}

int parse(char *inputChar){
  char current = *inputChar;
  int stringLen = strlen(inputChar);
  
  if((current=='p')||(current=='q')||(current=='r')){
    char next = inputChar[1];
    if(next == '\0'){
      return 1;
    }
  }

  else if(inputChar[0] == '-'){
    if(parse(returnTail(inputChar))!=0){
      return 2;
    }
  }

  else if(inputChar[0] == '(' && inputChar[stringLen - 1] == ')'){
    int bool = compareBracketAndConnectives(inputChar);
    if(bool == 1){
      if(partone(inputChar) != NULL && parttwo(inputChar) != NULL){
        return 3;
      }
    }
  }

  else {
    return 0;
  }
}

char getConnective(char *inputChar){
  int brackets = 0, index = 0;
  while(inputChar[index+1] != '\0'){
    char current = inputChar[index];
    if(current == '(') {
      brackets++;
      index ++;
      }
      
    else if(current == ')') {
      brackets --;
      index ++;
      }

    else if ((current == '>' || current == '^' || current == 'v') && brackets == 1) return current;

    else index++;
  }
  return '@';
}

//Method to remove any occurence of double negation before we pass the formula through to makeTree method
char *removeDoubleNegation(char *inputChar){
  int length = strlen(inputChar), index = 0, doubleNegations = 0;
  while(inputChar[index+1] != '\0'){
    char current = inputChar[index];
    char next = inputChar[index + 1];
    if(current == '-'){
      if(next == '-'){
        doubleNegations += 2;
        index ++;
      }
      else{
        index ++;
      }
    }
    else{
      index ++;
    }
  }
  char *noDoubleNegation = (char*)malloc(sizeof(char) * (length - doubleNegations + 1));
  index = 0;
  int newIndex = 0;
  while(inputChar[index + 1] != '\0'){
    char current = inputChar[index]; 
    char next = inputChar[index + 1];
    if(current == '-'){
      if(next == '-'){
        index += 2;
      }
      else{
        noDoubleNegation[newIndex] = current;
        newIndex ++;
        index ++;
      }
  }
  else{
    noDoubleNegation[newIndex] = current;
    newIndex ++;
    index ++;
    }
  }
  char last = inputChar[index];
  noDoubleNegation[newIndex] = last;
  newIndex ++;
  noDoubleNegation[newIndex] = '\0';
  return noDoubleNegation;
}

char *addNegation(char *inputChar){
  // length adds 1 to the string length to compensate for the negation 
  int length = strlen(inputChar) + 1, newIndex = 0, oldIndex = 0;
  char *returnList = (char*) malloc(length * sizeof(char));
  returnList[newIndex++] = '-';
  while(inputChar[oldIndex] != '\0'){
    char current = inputChar[oldIndex];
    returnList[newIndex] = current;
    newIndex ++; oldIndex ++;
  }
  returnList[newIndex] = '\0';
  return returnList;
}

// leftRight is the branch variable: 1 - left, 2 - right
// We keep on appending to the left of the tree if the formula is alpha
// We only append to the right when the forumla is beta
void makeNewNode(struct tree *tree, char *inputChar, int leftRight){
  struct tree *newNode = malloc(sizeof(struct tree));
  newNode->formula = inputChar;
  newNode->left = NULL;
  newNode->right = NULL;
  newNode->parent = tree;
  if(leftRight == 1) (*tree).left = newNode;
  else if(leftRight == 2) (*tree).right = newNode;
}

void addNode(struct tree *tree, char *inputChar){
  if((*tree).left != NULL){
    addNode((*tree).left, inputChar);
  }
  else{
    makeNewNode(tree,inputChar,1);
  }
  if((*tree).right != NULL){
    addNode((*tree).right, inputChar);
  }
}

void alphaOrBeta(struct tree *tree, char *first, char *second, int type){
  // type is the variable that determines the formula type: 2 - alpha, type 3 - beta
  // We do not have to handle double negation as we already removed it before parsing the formula 
  if(type == 2){
    addNode(tree,first);
    addNode(tree,second);
  }
  else if (type == 3){
    if((*tree).left == NULL){
      makeNewNode(tree,first,1);
      makeNewNode(tree,second,2);
    }
    else{
      alphaOrBeta((*tree).left,first,second,3);
      if((*tree).right != NULL){
        alphaOrBeta((*tree).right, first,second,3);
      }
    }
  }
}

void makeTree(struct tree *tree){
  char *formula = (*tree).formula;
  int formulaType = parse(formula);

  //Binary formulas 
  if(formulaType == 3){
    char connective = getConnective(formula);
    if(connective == '^') alphaOrBeta(tree,partone(formula),parttwo(formula),2);
    else if(connective == 'v') alphaOrBeta(tree,partone(formula), parttwo(formula), 3);
    // a > b <=> -a v b
    else if(connective == '>') alphaOrBeta(tree,addNegation(partone(formula)), parttwo(formula), 3);
  }

  //Negation formulas that a followed by a binary formula
  if (formulaType == 2 && parse(returnTail(formula)) == 3){
    char connective = getConnective(returnTail(formula));
    // -( a ^ b ) <=> -a v -b
    if(connective == '^') alphaOrBeta(tree,addNegation(returnTail(partone(formula))),addNegation(parttwo(formula)),3);
    // -( a v b ) <=> -a ^ -b
    else if(connective == 'v') alphaOrBeta(tree,addNegation(returnTail(partone(formula))), addNegation(parttwo(formula)), 2);
    // -( a > b ) <=> a ^ - b
    else if(connective == '>') alphaOrBeta(tree,returnTail(partone(formula)), addNegation(parttwo(formula)), 2);
  }
}

void complete(struct tree *tree){
  if(tree != NULL){
    makeTree(tree);
    complete((*tree).left);
    complete((*tree).right);
  }
}

//literals variable contains the literals currently present in the parent node
//It represents p, q, r and -p, -q, -r
int closed(struct tree *tree, int *oldLiterals){
  int newLiterals[6];
  //Copies the literals from the previous branch 
  memcpy(newLiterals, oldLiterals, 6*sizeof(int));
  char *formula = (*tree).formula;
  int parseVal = parse(formula);

  if(parseVal == 0) return 0;

  if(parseVal == 1){
    char current = formula[0];
    if(current == 'p'){newLiterals[0] = 1;}
    else if(current == 'q'){ newLiterals[1] = 1;}
    else if(current == 'r'){ newLiterals[2] = 1;}
  }
  
  if((parseVal == 2) && (parse(returnTail(formula)) == 1)){
    char current = formula[1];
    if(current == 'p'){newLiterals[3] = 1;}
    else if(current == 'q'){ newLiterals[4] = 1;}
    else if(current == 'r'){ newLiterals[5] = 1;}
  }

  //Literals are compared E.g. if p and -p is found in the brach, we close it.
  //p and -p
  if(newLiterals[0] == 1 && newLiterals[3] == 1) return 1;
  //q and -q
  if(newLiterals[1] == 1 && newLiterals[4] == 1) return 1;
  //r and -r
  if(newLiterals[2] == 1 && newLiterals[5] == 1) return 1;

  //We finish iterating recursively through the tree and cannot close it
  if ((*tree).left == NULL && (*tree).right == NULL) return 0;

  //We iterate recursively if there is still a left branch present
  if((*tree).left != NULL && (*tree).right == NULL) return closed((*tree).left, newLiterals);

  //We iterate recursively if a left and right branch is present
  if((*tree).left != NULL && (*tree).right != NULL){
    int closeLeft = closed((*tree).left, newLiterals);
    int closeRight = closed((*tree).right, newLiterals);
    if(closeLeft == 1 && closeRight == 1) return 1;
    else return 0;
  }

  return 0;
}


int main()

{ /*input 10 strings from "input.txt" */


  char *name = malloc(Fsize);
  char *store = malloc(Fsize);
  FILE *fp, *fpout;
 
  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<inputs;j++)
    {
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name))
      {
          case(0): fprintf(fpout, "%s is not a formula.  \n", name);break;
          case(1): fprintf(fpout, "%s is a proposition. \n ", name);break;
          case(2): fprintf(fpout, "%s is a negation.  \n", name);break;
          case(3):fprintf(fpout, "%s is a binary. The first part is %s and the second part is %s  \n", name, partone(name), parttwo(name));break;
          default:fprintf(fpout, "What the f***!  ");
      }

      store = name;
      name = removeDoubleNegation(name);
      if (parse(name)!=0)
      {
        /* here you should initialise a theory S with one formula (name) in it and then initialise a tableau t with on theory (S) in it*/
        /* then you should call a function that completes the tableau t*/
        struct tree newTree;
        newTree.formula = name;
        newTree.parent = NULL;
        newTree.left = NULL;
        newTree.right = NULL;
        int literal[6] = {0,0,0,0,0,0};
        complete(&newTree);
        if (closed(&newTree, literal)) fprintf(fpout,"%s is not satisfiable\n", store);
        else fprintf(fpout,"%s is satisfiable\n", store);
      }
      else  fprintf(fpout, "I told you, %s is not a formula.\n", store);
    }



  fclose(fp);
  fclose(fpout);
  free(name);
  free(store);
  return(0);
}