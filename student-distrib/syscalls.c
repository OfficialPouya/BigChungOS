//syscalls.c


int32_t execute(const uint8_t* command){
    int command_index, i, j = 0; // variables to be used as indices 
    uint8_t filename[FILENAME_LEN]; // array to hold file name 
    //1. PARSE COMMAND FOR FILE NAME

    //check for valid command 
    if(command == NULL){
        return -1; 
    }
    // iterate through initial white space 
    while(command[command_index] == ' '){
        command_index++;
    }
    // get file name from command 
    while(command[command_index] != ' ' && command[command_index] != '\0'){
        filename[i] = command[command_index];
        i++;
        command_index++;
    }
    // add newline char at end of file name
    filename[i] = '\0';

    //iterate through second set of white space before args
    while(command[command_index] == ' '){
        command_index++;
    }
    // stores args in pcb 
    while(command[command_index] != ' ' && command[command_index] != '\0'){
        //store args in pcb (pcb->args[j] = command[command_index])??
        j++;
        command_index++;
    }
    // may need to append newline char not positive tho

    //2. CHECK FOR EXE
    
}