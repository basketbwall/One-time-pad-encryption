#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Modify the sample.txt to change the message for encryption/decryption

void print_choices();
char* read_file(int len, char* path);
void write_file(int len, char* in_array, char* path);
char* make_rand_key(int length, char* key_storage);
void encrypt(char* clear_file, char* key_file, char* encrypted_path);
void decrypt(char* key_file, char* encrypted, char* decrypted_path);

int main()
{	
	char* regular_text = "sample.txt";
	char* key_file = "key.txt";
			

	print_choices();
	int response;
	scanf("%d", &response);
	int size;
	while (response != 3) { // response is either 1 or 2
		if (response == 1) { //user wishes to encrypt a message lol
			size = 0;
			FILE* regular_file = fopen(regular_text, "r");
			while (getc(regular_file) != EOF) {
				size++;
			}
			rewind(regular_file);
			char* key_array;
			make_rand_key(size, key_array);

			char* encrypted_name = "encrypted.txt";
			
			encrypt(regular_text, key_file, encrypted_name);
			fclose(regular_file);
		} else if (response == 2) {
			char* encrypted = "encrypted.txt";
			char* decrypted_path = "decrypted.txt";
			decrypt(key_file, encrypted, decrypted_path);
		} else {
			printf("You did not enter a valid choice.");
		}
		print_choices();
		scanf("%d", &response);
	}

	/* 
	// testing writing a file given character of things to write and string of the path
	char x[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
	char* y = "text.txt";
	write_file(4, x, y);

	// testing random key function
	char* storing_key;
	make_rand_key(5, storing_key);
	FILE *in = fopen("sample.txt", "r");
	// need to create path, keypath, and make a pointer
	char* path = "encrypted_sample.txt";
	char* keypath = "key.txt";
	FILE *key = fopen(keypath, "r");
	encrypt(in, key, path);

	char* result_path = "decrypted_sample.txt";
	// testing decrypt
	FILE *encrypted = fopen("encrypted_sample.txt", "r");
	decrypt(key, encrypted, result_path);
	
	// testing read function
	char* reading_path = "minions.txt";
	char* text_read = read_file(4, reading_path);
	printf("Here is the string that was returned: %s\n", text_read);
	
	*/

	/*
	char test[100];
	int x = read_file(test);
	printf("%d\n", x);
	int i = 0;
	while(test[i] != '\0') {
		printf("%c", test[i]);
		i++;
	}
	printf("\n");
	write_file();
	
	char* keyholder;
	keyholder = make_rand_key(x, keyholder);
	printf("printing a string %s\n", keyholder);
	*/
}


char* read_file(int len, char* path) // takes in length to be read, and gives path of file to be read
{
	if (len == 0) {
		FILE *in = fopen(path, "r");
		char current = getc(in);
		int size = 0;
		while (current != EOF) {
			size++;
			current = getc(in);
		}
		char* return_array = (char*)malloc((size)*sizeof(char));
		
		rewind(in);
		current = getc(in);
		int y = 0;
		while (current != EOF) {
			return_array[y] = current;
			y++;
			current = getc(in);
		}
		return return_array;
	} else { //len is not zero	
		FILE *in = fopen(path, "r");
		char current = getc(in);
		char* return_array = (char*)malloc((len)*sizeof(char));
		int y = 0;
		while (y < len) {
			return_array[y] = current;
			y++;
			current = getc(in);
		}
		return return_array;
	}
	/*
	FILE *in = fopen("sample.txt", "r");
	int len = 0;
	while (getc(in) != EOF) { // while loop to grab how many characters in file
		len++;
	}
	rewind(in);
	
	int x = 0;
	
	for(; x<len; x++) { // loop starts copying info from file to an array
		file_text[x] = getc(in);
	}
	file_text[len] = '\0';
	
	fclose(in);
	return len;
	*/
}

void write_file(int len, char* in_array, char* path) //takes length of chars to write, in_array is array of items we write, and path is just name of the file
{
	FILE* out;
	out = fopen(path, "w");
	
	if (len == 0) {
		int i=0;
		while(in_array[i] != '\0') {			
			putc(in_array[i], out);
			i++;
		}
	} else {
		int i=0;
		while(i < len) {
			putc(in_array[i], out);
			i++;
		}
	}

	fclose(out);
}

char* make_rand_key(int length, char* key_storage) {
	key_storage = (char *)malloc((length+1)*sizeof(char)); //length+1
	srand(time(0));
	int i;
	for (i=0; i<length; i++) {
		char newchar = (char) rand()%256; // casted explicitly as char
		if (newchar == EOF) {
			newchar = newchar - 1;
		} else if (newchar == '\0') {
			newchar = 1;
		}
		key_storage[i] = newchar;
	
	}
//	printf("length of key should be %d\n", length);
	key_storage[i] = '\0'; 
	char* path_name = "key.txt";
	write_file(0, key_storage, path_name);
//	printf("The generated key is %s\n", key_storage);
	return key_storage;
}

void encrypt(char* clear_file, char* key_file, char* encrypted_path) {
	FILE* original = fopen(clear_file, "r");
	FILE* key = fopen(key_file, "r");
	int len = 0;
	while (getc(key) != EOF) {
		len++;
	}
//	printf("length generated from encrypt function %d\n", len);
	rewind(key);
	
	char encrypted_message[len];
	int y = 0;
	rewind(original);
	char letter_from_original = getc(original);
	char letter_from_key = getc(key);
	while (y < len) {
		encrypted_message[y] = letter_from_original ^ letter_from_key;
		letter_from_original = getc(original);
		letter_from_key = getc(key);
		y++;
	}
	encrypted_message[y] = '\0';
//	printf("The length of this encrypted file shall be %d\n", y);
	write_file(y, encrypted_message, encrypted_path);
}


void decrypt(char* key_file, char* encrypted, char* decrypted_path) {
	FILE* key = fopen(key_file, "r");
	FILE* encrypted_file = fopen(encrypted, "r");
	int len = 0;
	rewind(key);
	rewind(encrypted_file);

	while (getc(key) != EOF && getc(encrypted_file) != EOF) {
		len++;
	}
	
//	printf("length to be determined is %d\n", len);
	rewind(key);
	rewind(encrypted_file);
	
	int y = 0;
	char decrypted_message[len];
	char letter_from_encrypted = getc(encrypted_file);
	char letter_from_key = getc(key);
	while (y < len) {
		decrypted_message[y] = letter_from_encrypted ^ letter_from_key;
		letter_from_encrypted = getc(encrypted_file);
		letter_from_key = getc(key);
		y++;
	}
	decrypted_message[y] = '\0';
//	printf("written about %d characters\n", y);
	write_file(0, decrypted_message, decrypted_path);
}

void print_choices()
{
	printf("Encrypt a file:%3d\n", 1);
	printf("Decrypt a file:%3d\n", 2);
	printf("Exit:%13d\n", 3);
	printf("Enter a choice: ");		
}
