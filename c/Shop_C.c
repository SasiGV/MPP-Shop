/* SHOP Assignment

Functionality
The shop CSV should hold the initial cash value for the shop.
Read in customer orders from a CSV file.
– That file should include all the products they wish to buy and in what quantity.
– It should also include their name and their budget.
The shop must be able to process the orders of the customer.
– Update the cash in the shop based on money received.
* It is important that the state of the shop be consistent.
* You should create customer test files (CSVs) which cannot be completed by the shop e.g. customer wants 400
loaves of bread but the shop only has 20, or the customer wants 2 cans of coke but can only afford 1.
* If these files don’t exist marks penalties will be applied.
– Know whether or not the shop can fill an order.
* Thrown an appropriate error.
Operate in a live mode, where the user can enter a product by name, specify a quantity, and pay for it. The user should
be able to buy many products in this way.
Notes
The above described functionality should be completed in Python and C. This is to be done in a procedural programming
style.
The second part of the assessment involves replicating the functionality of the shop in Java. This must be done in an
Object Oriented manner.
You must complete a short report, around 3-5 pages, which compares the solutions achieved using the procedural
approach and the object oriented approach.
The live mode, and the input files, should have the exact same behaviour in ALL implementations.
– For example I should be able to use the Java implementation in the same way as the C one i.e. same CSV files,
and the same process when doing an order in live mode.
– The “user experience” of each implementation should be identical.

*/

// C - Program using Procedural Programming 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

#define MAX_FILE_NAME_LENGTH 260
#define MAX_INPUT_FILES 20
#define MAX_PRODUCTS_IN_STOCK 20
//Global variables
char *DIRECTORY_PATH="../";


//Defind Getline function

int getline(char **lineptr, size_t *n, FILE *stream)
{
static char line[256];
char *ptr;
unsigned int len;

   if (lineptr == NULL || n == NULL)
   {
      errno = EINVAL;
      return -1;
   }

   if (ferror (stream))
      return -1;

   if (feof(stream))
      return -1;
     
   fgets(line,256,stream);

   ptr = strchr(line,'\n');   
   if (ptr)
      *ptr = '\0';

   len = strlen(line);
   
   if ((len+1) < 256)
   {
      ptr = realloc(*lineptr, 256);
      if (ptr == NULL)
         return(-1);
      *lineptr = ptr;
      *n = 256;
   }

   strcpy(*lineptr,line); 
   return(len);
}






//Structures
struct Product {
	char* name;
	double price;
};

struct ProductStock {
	struct Product product;
	int quantity;
};

struct Shop {
	double cash;
	struct ProductStock stock[MAX_PRODUCTS_IN_STOCK];
	int index;
};

struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};

//Functions' Declarations
void printProduct(struct Product);
void printCustomer(struct Customer, struct Shop *);
struct Shop createAndStockShop();
void printShop(struct Shop);
double findProductPrice(char *, struct Shop *);
int findProductQuantity(char *, struct Shop *);
unsigned int findProduct(char *, struct Shop *);
struct Customer readCustomer(char [],struct Shop *);
static int getAllFiles(char *[]);

//Functions' Definitions
void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
	//printf("-------------\n");
}

void printCustomer(struct Customer c, struct Shop *s)
{
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n\n", c.name, c.budget);
	double total = 0.0;
	double cost = 0.0;
	struct temp_data {
		char* product_name;
		int available_qty;
		int need_qty;
	};
	struct temp_buying_data {
		char* product_name;
		int qty;
		double sub_total;
	};
	struct temp_out_of_stock {
		struct temp_data products[MAX_PRODUCTS_IN_STOCK];
		int index;
	};
	struct temp_active_products {
		struct temp_buying_data products[MAX_PRODUCTS_IN_STOCK];
		int index;
	};
	struct temp_out_of_stock out_of_stock={.index=0};
	struct temp_active_products active_products_list={.index=0};

	for(unsigned int i = 0; i < c.index; i++)
	{
		//check whether the product exisits in stock or not
		unsigned int check_val = findProduct(c.shoppingList[i].product.name,s);
		if (check_val == 0)
		{
            printf("\n");
            printf("Error no %s in stock. List item is ignored\n",c.shoppingList[i].product.name);
		}
        else
		{
			printProduct(c.shoppingList[i].product);
			printf("\n%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
			if (findProductQuantity(c.shoppingList[i].product.name,s) < c.shoppingList[i].quantity)
			{
				struct temp_data data = {.product_name=c.shoppingList[i].product.name,
										.available_qty=findProductQuantity(c.shoppingList[i].product.name,s),
										.need_qty=c.shoppingList[i].quantity};
				out_of_stock.products[out_of_stock.index] = data;
				out_of_stock.index = (out_of_stock.index)+1;
			}
			cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
			struct temp_buying_data buying_data = {.product_name=c.shoppingList[i].product.name,
													.qty=c.shoppingList[i].quantity,
													.sub_total=cost};
			active_products_list.products[active_products_list.index] = buying_data;
			
			active_products_list.index = (active_products_list.index)+1;
			total+=cost;
			printf("The cost to %s will be $%.2f\n\n", c.name, cost);
		}
	}
	printf("--------------------------------------------------\n");
    printf("The total cost to %s will be $%.2f\n", c.name, total);
	
	if (out_of_stock.index == 0)
	{
		if (c.budget >= total)
		{
            printf("--------------------------------------------------\n");
            printf("SUCCESS !\n");
            printf("%s's budget is %.2f\n",c.name,c.budget);
            printf("%s has enough money\n",c.name);
            printf("total of %.2f will be deducted from %s's budget\n",total,c.name);
			
            //updating the stock
			for(unsigned int k = 0; k < s->index; k++)
			{
				for(unsigned int m = 0; m < active_products_list.index; m++)
					if (strcmp(s->stock[k].product.name, active_products_list.products[m].product_name)==0)
						s->stock[k].quantity = (s->stock[k].quantity - active_products_list.products[m].qty);
			}
                        
            c.budget = c.budget - total;
            s->cash = s->cash + total;
            printf("Remaining money in %s's budget is %.2f\n",c.name,c.budget);
            printf("total of %.2f added to shop\n",total);
            printf("Shop has %.2f\n",s->cash);
		}
		else
		{
			printf("--------------------------------------------------\n");
            printf("SORRY !\n");
            printf("%s's budget is %.2f\n",c.name,c.budget);
            printf("The total cost of all items is ...! %.2f\n",total);
            printf("%s does not have enough money\n",c.name);
            printf("total of 0 will be deducted from %s's budget\n",c.name);
            printf("%s's budget is %.2f\n",c.name,c.budget);
            printf("total of 0 added to shop\n");
            printf("Shop has %.2f in cash\n",s->cash);
		}
	}
	else
	{
		for(unsigned int j = 0; j < out_of_stock.index; j++)
		{
			printf("Not enough %s in stock\n",out_of_stock.products[j].product_name);
			printf("%d %s in stock\n",out_of_stock.products[j].available_qty,out_of_stock.products[j].product_name);
			printf("%s wants %d %s\n",c.name,out_of_stock.products[j].need_qty,out_of_stock.products[j].product_name);
			printf("Please revise your order and upload again!\n");
		}
	}	
}

struct Shop createAndStockShop()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("../stock.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

	read = getline(&line, &len, fp);
	float cash = atof(line);
	// printf("cash in shop is %.2f\n", cash);
	
	struct Shop shop = { cash };

    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s IS A LINE", line);
		char *n = strtok(line, ",");
		char *p = strtok(NULL, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		double price = atof(p);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		struct Product product = { name, price };
		struct ProductStock stockItem = { product, quantity };
		shop.stock[shop.index++] = stockItem;
		// printf("NAME OF PRODUCT %s PRICE %.2f QUANTITY %d\n", name, price, quantity);
    }
	
	return shop;
}

void printShop(struct Shop s)
{
	printf("Shop has %.2f in cash\n", s.cash);
	printf("The Shop has the following stock which will be checked against your order\n\n");
	for (int i = 0; i < s.index; i++)
	{
		printProduct(s.stock[i].product);
		printf("The shop has %d of the above\n\n", s.stock[i].quantity);
	}
}

double findProductPrice(char *product_name, struct Shop *s)
{
	double product_price = 0.0;
	for (int i = 0; i < s->index; i++)
	{
		if (strcmp(product_name,s->stock[i].product.name) == 0)
		{
			product_price = s->stock[i].product.price;
			break;
		}
	}
	return product_price;
}

int findProductQuantity(char *product_name, struct Shop *s)
{
	unsigned int product_qty = 0.0;
	for (int i = 0; i < s->index; i++)
	{
		if (strcmp(product_name,s->stock[i].product.name) == 0)
		{
			product_qty = s->stock[i].quantity;
			break;
		}
	}
	return product_qty;
}

unsigned int findProduct(char *product_name, struct Shop *s)
{
	
	for (int i = 0; i < s->index; i++)
	{
		if (strcmp(product_name,s->stock[i].product.name) == 0)
			return 1;
	}
	return 0;
}

struct Customer readCustomer(char file_name[MAX_FILE_NAME_LENGTH],struct Shop *s)
{
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen(file_name, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
	
	read = getline(&line, &len, fp);
	char *n = strtok(line, ",");
	char *b = strtok(NULL, ",");
	char *customer_name = malloc(sizeof(char) * 50);
	strcpy(customer_name, n);
	double customer_budget = atof(b);
	
	struct Customer c = {.name = customer_name, .budget = customer_budget};

    while ((read = getline(&line, &len, fp)) != -1)
	{
		char *n = strtok(line, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		double price = findProductPrice(name,s);
		struct Product product = { name, price };
		struct ProductStock product_stock = { product, quantity };
		c.shoppingList[c.index++] = product_stock;
    }
	
	return c;
}

static int getAllFiles(char *(file_names)[MAX_FILE_NAME_LENGTH])
{
	DIR *dir;
    struct dirent *de;
    dir = opendir("./"); //open directory
	if (dir == NULL)	//direcotry couldn't open
        exit(EXIT_FAILURE);
	int total_files=0;
    while(dir)	//read directory
    {	
		if ((de = readdir(dir)) != NULL)
		{
			//if (de->d_type == DT_REG)	//only get regular files
			//{
				const char *ext = strrchr(de->d_name,'.');
				if(strcmp(ext, ".csv") == 0)
				{
					file_names[total_files] = (char*) malloc(strlen(de->d_name)+1);
					strcpy(file_names[total_files],de->d_name);
					printf("FineNamefrom the array %s", file_names[total_files]);
					total_files++;
				}
			//}
		}
		else
		{
			closedir(dir);
			return (total_files-1);
		}
	} 
}

int main(void) 
{
	unsigned int active = 1;
	struct Shop shop = createAndStockShop();
	char *all_csv_files[MAX_FILE_NAME_LENGTH]={""};
	int total_csv_files = getAllFiles(all_csv_files);
	while(active)
	{
		printf("-----------------------------------\n");
		printf("Welcome to the Little Princess shop\n");
		printf("-----------------------------------\n");
		
		printf("");
		printf("Please select your option:\n");
		printf("Press 1 to see the list of available products in the shop:\n");
		printf("Press 2 to add your shopping list manually:(Refer option 1 for the list of products)\n");
		printf("Press 3 to view the customer file :\n");
		printf("Press 4 to view the customer_not_enough_money file :\n");
		printf("Press 5 to view the customer_too_much_bread file :\n");
		printf("Press 0 to exit:\n");
		printf("");

		
		
		//Take user input
		unsigned int answer;
		char temp_buf[4] = {0};
		printf("Option: ");
		scanf("%s",temp_buf);
		
		if (isdigit(temp_buf[0]))
		{
			answer = atoi(temp_buf);
			printf("\n");
			char customer_csv_path[MAX_FILE_NAME_LENGTH]="";
			struct Customer c;
			unsigned int custom_active = 1;
			double custom_budget = 0.0;
			//struct temp_active_products active_products_list={.index=0};
			//Take actions according to the user input
			switch (answer)
			{
				case 1:
					printf("\n");
					printf("---------------------------------------------------\n");
					printShop(shop);
					printf("---------------------------------------------------\n");
		
					//printf("Print file names\n");
					//printf("First File %s", all_csv_files[0]);
					//printf("\n");
					//printf("First File %s", all_csv_files[1]);
					
					break;
				case 2:
					// Get input from User 
					printf("---------------------------------------------------\n");
					printf("List of available stock in the shop\n");
					printShop(shop);
					printf("---------------------------------------------------\n");
					
					//custom_budget = input("Please enter your budget and items : ");
					//active_product_list = []
					
					struct temp_data {
					char* product_name;
					int available_qty;
					int need_qty;
					};
					struct temp_buying_data {
						char* product_name;
						int qty;
						double sub_total;
					};
					struct temp_out_of_stock {
						struct temp_data products[MAX_PRODUCTS_IN_STOCK];
						int index;
					};
					struct temp_active_products {
						struct temp_buying_data products[MAX_PRODUCTS_IN_STOCK];
						int index;
					};
					struct temp_out_of_stock out_of_stock={.index=0};
					struct temp_active_products active_products_list={.index=0};
								
					
					
					printf("Type your budget : ");
					scanf("%lf",&custom_budget);
					while (custom_active == 1)
					{
						char custom_product[MAX_FILE_NAME_LENGTH]="";
						printf("Enter a product from the list above: ");
						
						scanf(" %[^\n]s",custom_product);
						if (findProduct(custom_product,&shop)!=1)
						{
							printf("Sorry this product  : %s, is not available in the stock\n",custom_product);
						}
						else
						{
							//printf("Entered Product is %s \n",custom_product);
							int custom_count=0;
							double custom_sub_total = 0.0;
							int available_qty = findProductQuantity(custom_product,&shop);
							
							//Get the second input
							printf("How many do you want? : ");
							scanf("%d",&custom_count);
							if (available_qty >= custom_count)
							{
								custom_sub_total = (findProductPrice(custom_product,&shop) * custom_count);
								printf("The cost to you will be %.2f\n",custom_sub_total);
								struct temp_buying_data buying_data = {.product_name=custom_product,
																.qty=custom_count,
																.sub_total=custom_sub_total};
								active_products_list.products[active_products_list.index] = buying_data;
								active_products_list.index = (active_products_list.index)+1;
							}
							else
							{
								printf("---------------------------------------------------\n");
								printf("\n");
								printf("SORRY ! \n");
								printf("Not enough %s in stock\n",custom_product);
								printf("Total available %d %s in stock\n",available_qty,custom_product);
								printf("You want %d of %s\n",custom_count, custom_product);
								printf("Please your revise order and try again!\n");
							}
						}
						char close_tag[4] = "Y";
						printf("Would you like another product? Y/N ");
						scanf("%s",close_tag);
						if ((strcmp(close_tag,"N")==0) || (strcmp(close_tag,"n")==0))
						{
							custom_active = 0;
						}
						
						
					}
					printf("Summary\n");
					if (active_products_list.index > 0)
					{
						double total = 0;
						for(unsigned int m = 0; m < active_products_list.index; m++)
							total = total + active_products_list.products[m].sub_total;
						if (total > custom_budget)
						{
							printf("---------------------------------------------------\n");
							printf("SORRY! \n");
							printf("Your budget is ...\n");
							printf("The total cost of all items is ...! %.2f\n",total);
							printf("You do not have enough money\n");
							printf("Total of 0 will be deducted from your budget\n");
							
						}
						else
						{
							printf("---------------------------------------------------\n");
							printf("SUCCESS! \n");
							printf("Your budget is %.2f\n",custom_budget);
							printf("You have enough money\n");
							printf("Total of %.2f will be deducted from your budget\n",total);
							printf("Total of %.2f will be added to shop\n",total);
							
							//update the stock
							for(unsigned int k = 0; k < shop.index; k++)
							{
								for(unsigned int m = 0; m < active_products_list.index; m++)
									if (strcmp(shop.stock[k].product.name, active_products_list.products[m].product_name)==0)
										shop.stock[k].quantity = (shop.stock[k].quantity - active_products_list.products[m].qty);
							}
							double shop_total = 0.0;
							shop_total = shop.cash + total;
							shop.cash = shop_total;
							custom_budget = custom_budget - total;
							
							printf("Shop has %.2f in cash\n",shop_total);
							printf("\n");
						}	
					}
					else
					{
						printf("No more items left in your list to continue shopping\n");
					}
					printf("---------------------------------------------------\n");
					break;					
					

				case 3:
					printf("---------------------------------------------------\n");
					strcat(customer_csv_path,"../customer.csv");
					//strcat(customer_csv_path,all_csv_files[answer-2]);
					printf("you selected %s as a current customer data\n",customer_csv_path);
					printf("\n");
					// Option 2 to Read Customer File
					c = readCustomer("../customer.csv",&shop);
					
					//c = readCustomer(customer_csv_path,&shop);
					//printCustomer(c);
					
					printCustomer(c,&shop);
					printf("\n");
					printf("---------------------------------------------------\n");
					break;
				case 4:
					printf("---------------------------------------------------\n");
					strcat(customer_csv_path,"../customer_not_enough_money.csv");
					//strcat(customer_csv_path,all_csv_files[answer-2]);
					printf("you selected %s as a current customer data\n",customer_csv_path);
					printf("\n");
					// Option 2 to Read Customer File - customer_not_enough_money
					c = readCustomer("../customer_not_enough_money.csv",&shop);
					//c = readCustomer(customer_csv_path,&shop);
					printCustomer(c,&shop);
					printf("\n");
					printf("---------------------------------------------------\n");
					break;
				case 5:
					printf("---------------------------------------------------\n");
					strcat(customer_csv_path,"../customer_too_much_bread.csv");
					//strcat(customer_csv_path,all_csv_files[answer-2]);
					printf("you selected %s as a current customer data\n",customer_csv_path);
					printf("\n");
					// Option 2 to Read Customer File - customer_too_much_bread
					c = readCustomer("../customer_too_much_bread.csv",&shop);
					//c = readCustomer(customer_csv_path,&shop);
					printCustomer(c,&shop);
					printf("\n");
					printf("---------------------------------------------------\n");
					break;
				case 0:
					printf("\n");
					printf("Thanks!... You have selected 0 to exit. Please visit the shop again");  
					printf("\n");					
					active = 0;
					break;
				default:
					printf("\n");
					printf("-----------------------------------------------------------------------------------\n");
					printf(" Selected option is not available. Please select one of the available options below\n");
					printf("###################################################################################\n");
					break;
			} 
		}
		else
		{
			printf("####################################\n");
			printf("NameError: Please Use Numbers Only\n");
			printf("####################################\n");
			
		}
		
	}
    return 0;
}