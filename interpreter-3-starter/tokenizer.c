#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "linkedlist.h"
#include "value.h"
#include "talloc.h"

//HELPER FUNCTIONS!!!!
bool isDigit(char charRead) {
    return (charRead >= '0') && (charRead <= '9');
}

bool isInteger(char* string){
    for (int i = 0; i < strlen(string); i++) {
        if(i == 0 && strlen(string) != 1){
            if(string[0] == '-' ||string[0] == '+'){
                continue;
            }
        }
        if (!isDigit(string[i])) {
            return false;
        }
    }
    return true;

}

bool isDouble(char* string) {
    int decCount = 0;
    for (int i = 0; i < strlen(string); i++) {
        if(i == 0 && strlen(string) != 1){
            if(string[0] == '-' ||string[0] == '+'){
                continue;
            }
        }
        if (!isDigit(string[i])) {
            if (string[i] == '.') {
                decCount++;
            }
            else {
                return false;
            }
        }
    }

    if (decCount == 1) {
        return true;
    }
    return false;
}

bool isLetter(char charRead) {
    return ((charRead <= 'z') && (charRead >= 'a')) || ((charRead <= 'Z') && (charRead >= 'A'));
}

bool isInitial(char charRead) {
    if(isLetter(charRead)){
        return true;
    }else if(charRead == '!' || charRead == '$' || 
    charRead == '%'|| charRead == '&'||charRead == '*'||
    charRead == '/'||charRead == ':'||charRead == '<' ||
    charRead == '='||charRead == '>'||charRead == '?' ||
    charRead == '~'||charRead == '_'||charRead == '^'){
        return true;
    }else{
        return false;
    }
}

bool isSymbol(char* string){
    if(strlen(string) == 1){
        if(string[0] == '+' || string[0] == '-'||isInitial(string[0])){
            return true;
        }else{
            return false;
        }
    }else if (isInitial(string[0])){
        for (int i = 1; i < strlen(string); i++) {
            if (isInitial(string[i]) || isDigit(string[i]) || string[i] == '.' ||
            string[i] == '+' || string[i] == '-') {
                continue;
            }
            else {
                return false;
            }
        }
        return true;
    }else{
        return false;
    }

}

bool isBoolean(char *string) {
    if (string[0] != '#') {
        return false;
    }
    if (strlen(string) != 2) {
        return false;
    }
    else if (string[1] == 't' || string[1] == 'f') {
        return true;
    }
    else {
        return false;
    }
}
//test

// Read all of the input from stdin, and return a linked list consisting of the
// tokens.
Value *tokenize(){
    char charRead;
    Value *list = makeNull();
    charRead = (char)fgetc(stdin);

    while (charRead != EOF) {

        if (charRead == '(') {
            Value* val = talloc(sizeof(Value));
            val -> type = OPEN_TYPE;
            val -> s = "(";
            list = cons(val, list);
        } else if (charRead == ')') {
            Value* val = talloc(sizeof(Value));
            val -> type = CLOSE_TYPE;
            val -> s = ")";
            list = cons(val, list);
        }  else if (charRead == ' ') {
            charRead = (char)fgetc(stdin);
            continue; //effectively skip
        }   else if (charRead == '\n') {
            charRead = (char)fgetc(stdin);
            continue; 
        }   else if (charRead == ';') {
            while (charRead != '\n' && charRead != EOF){
                charRead = (char)fgetc(stdin);
            }
            continue; 
            //indicating comment (but ok if included in string)
            //interate through the comment
        } else if (charRead == '"') {
            //string
            char token[301];
            int i = 1;
            token[0] = charRead;
            charRead = (char)fgetc(stdin);
            while(charRead != '"'){
                token[i] = charRead;
                i++;
                if(charRead == '\n' || charRead == EOF){
                    printf("%s", "Syntax error : untokenizeable");
                    texit(0);
                }
                charRead = (char)fgetc(stdin);
            }
            token[i] = charRead;
            token[i+1] = '\0';
            charRead = (char)fgetc(stdin);
            if(charRead != '('&& charRead != ')'&& 
            charRead != ' '&& charRead != ';'&& charRead != '\n'){
                printf("%s", "Syntax error : untokenizeable");
                texit(0);
            }
            Value* val = talloc(sizeof(Value));
            val -> type = STR_TYPE;
            val -> s = talloc(sizeof(char)*strlen(token)+1);
            strcpy(val->s,token);
            list = cons(val, list);
            continue; 
        } else {
            char token[301];
            int i = 1;
            token[0] = charRead;
            charRead = (char)fgetc(stdin);
            while(charRead != '('&& charRead != ')'&& 
            charRead != ' '&& charRead != ';'&& charRead != '\n'){
                token[i] = charRead;
                i++;
                charRead = (char)fgetc(stdin);
            }
            token[i] = '\0';
            if(isInteger(token)){
                Value* val = talloc(sizeof(Value));
                val -> type = INT_TYPE;
                val -> i = (int) strtol(token, (char **)NULL, 10);
                list = cons(val, list);
                continue;
            }else if(isDouble(token)){
                Value* val = talloc(sizeof(Value));
                val -> type = DOUBLE_TYPE;
                val -> d = strtod (token, (char **)NULL);
                list = cons(val, list);
                continue;
            }else if(isSymbol(token)){
                Value* val = talloc(sizeof(Value));
                val -> type = SYMBOL_TYPE;
                val -> s = talloc(sizeof(char)*strlen(token)+1);
                strcpy(val->s,token);
                list = cons(val, list);
                continue;
            }else if(isBoolean(token)){
                Value* val = talloc(sizeof(Value));
                 val -> type = BOOL_TYPE;
                 val -> s = talloc(sizeof(char)*strlen(token)+1);
                 strcpy(val->s,token);
                 list = cons(val, list);
                 continue;
            }else{
                printf("%s", "Syntax error : untokenizeable\n");
                texit(0);
            }

        }

        charRead = (char)fgetc(stdin);
    }


    Value *revList = reverse(list);
    return revList;
}


// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list){
    int i =1;
    while (list -> type != NULL_TYPE) {
        switch (car(list) -> type) {
            case INT_TYPE:
                printf("%i:integer \n", car(list) -> i);
                break;
            case DOUBLE_TYPE:
                printf("%f:double \n", car(list) -> d);
                break;
            case STR_TYPE:
                printf("%s:string \n", car(list) -> s);
                break;
            case BOOL_TYPE:
                printf("%s:boolean \n", car(list) -> s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol \n", car(list) -> s);
                break;
            case OPEN_TYPE:
                printf("%s:open \n", car(list) -> s);
                break;
            case CLOSE_TYPE:
                printf("%s:close \n", car(list) -> s);
                break;
            default:
                break;
        }
        list = cdr(list);
    }
}
