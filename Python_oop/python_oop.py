"""
 SHOP Assignment
 
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
– The “user experience” of each implementation should be identical.

"""

#below is solution in python in OOP style

from typing import List
import csv
import re
import os
from glob import glob

class Product:

    def __init__(self, name, price=0):
        self.name = name
        self.price = price
    
    def __repr__(self):
        str = f"PRODUCT NAME: {self.name}\n"
        str += f'PRODUCT PRICE {self.price}\n'
        return str

class ProductStock:
    
    def __init__(self, product, quantity):
        self.product = product
        self.quantity = quantity
    
    def name(self):
        return self.product.name
    
    def unit_price(self):
        return self.product.price

    def get_available_qty(self):
        return self.quantity
    
    def update_qty(self, qty):
        self.quantity = qty
        
    def cost(self):
        return self.unit_price() * self.quantity
        
    def __repr__(self):
        return f"{self.product}The Shop has {int(self.quantity)} of the above \n"

class Customer:

    def __init__(self, path):
        self.shopping_list = []
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            self.name = first_row[0]
            self.budget = float(first_row[1])
            for row in csv_reader:
                name = row[0]
                quantity = float(row[1])
                price = s.find_product_price(name)
                p = Product(name, price)
                ps = ProductStock(p, quantity)
                self.shopping_list.append(ps) 
                
    def calculate_costs(self, price_list):
        for shop_item in price_list:
            for list_item in self.shopping_list:
                if (list_item.name() == shop_item.name()):
                    list_item.product.price = shop_item.unit_price()
    
    def order_cost(self):
        cost = 0
        
        for list_item in self.shopping_list:
            cost += list_item.cost()
        
        return cost
    
    def __repr__(self):
        
        str = f"CUSTOMER NAME : {self.name}\n" + f"CUSTOMER BUDGET: {self.budget}\n"
        total = 0.0
        out_of_stock = []
        active_product_list = []

        for item in self.shopping_list:
            # Check weather the selected product in stock
            check_val = s.find_product_price(item.name())
            if check_val == 0:
                str += '\n'
                str += f'Error no {item.name()} in stock. List item is ignored'
            else:
                str += f'\nPRODUCT NAME: {item.name()}'
                str += f'\n{self.name} ORDERS {item.get_available_qty()} OF ABOVE PRODUCT'
                if int(s.find_product_qty(item.name())) < int(item.get_available_qty()):
                    data = {
                        "product_name" : item.name(),
                        "availble_qty" : int(s.find_product_qty(item.name())),
                        "need_qty": int(item.get_available_qty())
                    }
                    out_of_stock.append(data)

            cost = int(item.get_available_qty()) * float(item.unit_price())
            buying_data = {
                "product" : item.name(),
                "qty" : item.get_available_qty(),
                "sub_tot" : cost
            }
            active_product_list.append(buying_data)
            total = total + cost
            str += f'\nThe cost to {self.name} will be €{round(cost, 2)}'
        
        str += '\n--------------------------------------------------'
        str += f'\nThe total cost to {self.name} will be €{total}'

        if len(out_of_stock) == 0:
            if float(c.budget) >= float(total):
                str += '\n--------------------------------------------------'
                str += '\nSUCCESS !'
                str += f"\n{self.name}'s budget is {self.budget}"
                str += f"\n{self.name} has enough money"
                str += f"\ntotal of {total} will be deducted from {self.name}'s budget"

                # updating the stock
                for stock in s.stock:
                    for product in active_product_list:
                        if stock.name() == product['product']:
                            qty = int(stock.quantity) - int(product['qty'])
                            stock.update_qty(qty)
                            
                self.budget = float(self.budget) - float(total)
                shop_total = float(s.cash) + float(total)
                s.update_cash(shop_total)

                str += f"\n{self.name}'s budget is {self.budget}"
                str += f"\ntotal of {total} added to shop"
                str += f"\nShop has {s.cash}"
            else:
                str += '\n--------------------------------------------------'
                str += '\nSORRY !'
                str += f"\n{self.name}'s budget is {self.budget}"
                str += f'\nThe total cost of all items is ...! {float(total)}'
                str += f"\n{self.name} does not have enough money"
                str += f"\ntotal of 0 will be deducted from {self.name}'s budget"
                str += f"\n{self.name}'s budget is {self.budget}"
                str += f"\ntotal of 0 added to shop"
                str += f"\nShop has {s.cash} in cash"
        else:
            for out_stock in out_of_stock:
                str += f"\nNot enough {out_stock['product_name']} in stock"
                str += f"\n{out_stock['availble_qty']} {out_stock['product_name']} in stock"
                str += f"\n{self.name} want {out_stock['need_qty']} {out_stock['product_name']}"
                str += "\nPlease revise your order and upload again!"
        
        return str 
        
class Shop:
    
    def __init__(self, path):
        self.stock = []
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            self.cash = float(first_row[0])
            for row in csv_reader:
                p = Product(row[0], float(row[1]))
                ps = ProductStock(p, float(row[2]))
                self.stock.append(ps)

    def find_product(self, product_name):
        for item in self.stock:
            if str(product_name) == str(item.name()):
                return 1
        return 0

    def find_product_qty(self, product_name):
        for item in self.stock:
            if str(product_name) == str(item.name()):
                return item.get_available_qty()
        return 0
    
    def find_product_price(self, product_name):
        for item in self.stock:
            if str(product_name) == str(item.name()):
                return item.unit_price()
        return 0
    
    def update_cash(self, cash):
        self.cash = cash
    
    def __repr__(self):
        str = ""
        str += f'Shop has {self.cash} in cash\n'
        str += f'The Shop has the following stock which will be cheked against your order\n'
        str += f'\n'
        for item in self.stock:
            str += f"{item}\n"
        return str

s = Shop("../stock.csv")
active = True  # This is our on/off switch for the program. True is on and False is off.


while active:  # while active(True) - keep repeating everything inside
    
    print("---------------------------------------------------")
    print('Welcome to the Little Princess shop')
    print("---------------------------------------------------")
 
    PATH = "../"
    EXT = "*.csv"
    all_csv_files_ = [file
                    for path, subdir, files in os.walk(PATH)
                    for file in glob(os.path.join(path, EXT))]

    all_csv_files = []
    for file_path in all_csv_files_:
        all_csv_files.append(file_path)

    print('')    
    print('Please select your option:')
    print('Press 1 to see the list of available products in the shop:')
    print('Press 2 to add your shopping list manually:(Refer option 1 for the list of products)')
    print('Press 3 to select one of the customer files :')
    print('Press 0 to exit:')
    print('')

    try:  # This is a try statement used to handle errors
        IntOption = input("Option: ")  # This is a variable which stores the value a user enters
                                    # The input function makes the program wait for user input
                                    # Input returns integers, so letters and special characters cause errors
               
        if int(IntOption) == 1:
            print("-----------------------------------")
            print(s)
            print("-----------------------------------")
            print('')

        elif int(IntOption) == 2:

            custom_active = True
            print("List of available stock in the shop")
            print("")
            print(s)
            print("")
            print('Please enter your budget and items')
           
            custom_budget = input('Enter your budget : ')
            active_product_list = []

            while custom_active:

                custom_product = input('Enter a product from the list above: ')
                if not s.find_product(custom_product):
                    print('Sorry this product : {} is not available in the stock '.format(custom_product))
                    print("Please enter one of the available items")
                else:
                    custom_count = input('How many do you want? : ')

                    if int(s.find_product_qty(custom_product)) >= int(custom_count):

                        custom_sub_total = float(s.find_product_price(custom_product)) * int(custom_count)
                        print('Total cost for the selected item {} '.format(round(custom_sub_total, 2)))
                        data = {
                            "product" : custom_product,
                            "qty" : custom_count,
                            "sub_tot" : custom_sub_total
                        }
                        active_product_list.append(data)

                    else:
                        print("---------------------------------------------------")
                        print('')
                        print('Sorry ! ')
                        print(f'Not enough {custom_product} in stock ')
                        print(f'Total available{int(s.find_product_qty(custom_product))} {custom_product} in stock')
                        print(f'You want {custom_count} {custom_product}')
                        print('Please revise order and try again!')

                close_tag = input('Would you like to add another product? Y/N ')
                if (close_tag != 'Y') & (close_tag != 'y'):
                    custom_active = False
            
                
            if len(active_product_list) > 0:
                total = 0
                for product in active_product_list:
                    total = total + float(product['sub_tot'])
                
                if float(total) > float(custom_budget):
                    print("---------------------------------------------------")
                    print('SORRY! ')
                    print(f"Your budget is {custom_budget}")
                    print(f'The total cost of all items is ...! {total}')
                    print("You do not have enough money")
                    print(f"Total of 0 will be deducted from your budget")
                    print(f"Total of 0 added to shop")
                    #print(f"Shop has {round(float(s.cash), 2)} in cash")

                else:
                    print("---------------------------------------------------")
                    print('SUCCESS! ')
                    print(f"Your budget is {custom_budget}")
                    print("You have enough money")
                    print(f"Total of {round(total,2)} will be deducted from your budget")
                    print(f"Total of {round(total,2)} added to shop")

                    #update the stock
                    for stock in s.stock:
                        for product in active_product_list:
                            if stock.name() == product['product']:
                                qty = int(stock.quantity) - int(product['qty'])
                                stock.update_qty(qty)

                    shop_total = float(s.cash) + float(total)
                    s.update_cash(shop_total)
                    custom_budget = float(custom_budget) - float(total)
                                        
                    print(f"Shop has {round(shop_total, 2)} in cash")
                    print("")
            else:
                print('No more items left in your list to continue shopping')
            print("---------------------------------------------------")

        elif int(IntOption) == 3:
            print("---------------------------------------------------")
            print("List of available files:")
            CSVFile_active = True

            File_index = 1
            for file_name in all_csv_files:
                print ( File_index, all_csv_files[int(File_index) -1])
                File_index = File_index + 1
            
            
            while CSVFile_active:
                # Get the file number from the user
                File_count = input('Select the file from the given list : ')
                print("")

                if int(File_count) > int(len(all_csv_files)):
                    #If the user selects an item not in the list show a message
                    print("Sorry, selected item is not available:")
                else:
                    Index_adjust = 1
                    File_path = all_csv_files[int(File_count) - int(Index_adjust)]
                    print("File Path", str(File_path))
                    
                    if str(File_path) != "..\\stock.csv":
                        # Print the name of the file and items included in it
                  
                        print("---------------------------------------------------")
                        print(f'you have selected {File_path} :')
                        print('')
                        c = Customer(File_path)
                        print(c)
                        print('')
                        print("---------------------------------------------------")
                    
                    else:
                        #Print the stock file
                        print(f'you have selected {File_path} ')
                        print("-----------------------------------")
                        print(s)
                        print("-----------------------------------")
                
                selection_tag = input('Would you like to view another file? Y/N ')
                if (selection_tag != 'Y') & (selection_tag != 'y'):
                    CSVFile_active = False            

        elif int(IntOption) == 0:  # Option to exit the program
            print('Thanks!... You have selected 0 to exit. Please visit the shop again')               
            active = False
        else:  
            # Show error when the user selected different option that is not in the list
            print('-----------------------------------------------------------------------------------')
            print(" Selected option is not available. Please select one of the available options below")
            print('###################################################################################')

    except Exception as e: 
        print('')
        print(e)
        print("#####################################")
        print("For options: Please Use Numbers Only")
        print("#####################################")
        print('')



