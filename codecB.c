
void encode_codecB(char* str){
    int i = 0;
    while(str[i]){
        str[i] = str[i] + 3;
        i++;
    }
}
void decode_codecB(char* str){
    int i = 0;
    while(str[i]){
        str[i] = str[i] - 3;
        i++;
    }
}