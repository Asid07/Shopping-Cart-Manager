#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int HASH_TABLE_SIZE = 10; // Using k mod 10

// Product Structure
struct Product {
    int id;
    string name;
    float price;
};

// Inventory (Array of Products)
Product inventory[] = {
    {100, "Apple", 300},
    {101, "Banana", 100},
    {202, "Milk", 320},
    {301, "Bread", 150},
    {404, "Eggs", 400},
    {505, "Rice", 160},
    {606, "Sugar", 165},
    {707, "Salt", 30},
    {808, "Butter", 700},
    {909, "Cheese", 900}
};

int inventorySize = sizeof(inventory) / sizeof(inventory[0]);

// Doubly Linked List for Shopping Cart
struct CartItem {
    int id;
    string name;
    float price;
    int quantity;
    CartItem* prev;
    CartItem* next;
};

CartItem* head = nullptr;
CartItem* tail = nullptr;

// Hash Table for Fast Product Search
Product* hashTable[HASH_TABLE_SIZE] = {nullptr};

// Function to Initialize Hash Table
void initializeHashTable() {
    for (int i = 0; i < inventorySize; i++) {
        int hashIndex = inventory[i].id % HASH_TABLE_SIZE;
        hashTable[hashIndex] = &inventory[i];
    }
}

// Search Product in Inventory Using Hash Table
Product* searchProduct(int id) {
    int hashIndex = id % HASH_TABLE_SIZE;
    if (hashTable[hashIndex] && hashTable[hashIndex]->id == id) {
        return hashTable[hashIndex];
    }
    return nullptr;
}

// Add Item to Cart
void addToCart(int id, int quantity) {
    Product* product = searchProduct(id);
    if (!product) {
        cout << "Product not found!\n";
        return;
    }

    CartItem* newItem = new CartItem{product->id, product->name, product->price, quantity, nullptr, nullptr};
    
    if (!head) {
        head = tail = newItem;
    } else {
        tail->next = newItem;
        newItem->prev = tail;
        tail = newItem;
    }

    cout << "Added " << quantity << "x " << product->name << " to cart.\n";
}

// View Cart
void viewCart() {
    if (!head) {
        cout << "Cart is empty!\n";
        return;
    }

    CartItem* temp = head;
    float total = 0;
    cout << "\n=== Shopping Cart ===\n";
    while (temp) {
        float itemCost = temp->price * temp->quantity;
        total += itemCost;
        cout << temp->name << " x " << temp->quantity << " = Rs." << itemCost << endl;
        temp = temp->next;
    }
    cout << "Total: Rs." << total << endl;
}

// Remove Item from Cart
void removeFromCart(int id) {
    if (!head) {
        cout << "Cart is empty!\n";
        return;
    }

    CartItem* temp = head;
    while (temp && temp->id != id) {
        temp = temp->next;
    }

    if (!temp) {
        cout << "Item not found in cart!\n";
        return;
    }

    if (temp == head) {
        head = head->next;
        if (head) head->prev = nullptr;
    } else if (temp == tail) {
        tail = tail->prev;
        tail->next = nullptr;
    } else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    delete temp;
    cout << "Item removed from cart.\n";
}

// Merge Sort for Inventory Sorting
void merge(Product arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Product L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].price <= R[j].price) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(Product arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Display Sorted Inventory
void viewSortedInventory() {
    mergeSort(inventory, 0, inventorySize - 1);
    cout << "\n=== Inventory (by Price) ===\n";
    for (int i = 0; i < inventorySize; i++) {
        cout << inventory[i].name << " - Rs." << inventory[i].price << endl;
    }
}

// Print Cart to File
void printCart() {
    ofstream file("cart.txt");
    CartItem* temp = head;
    while (temp) {
        file << temp->id << " " << temp->name << " " << temp->quantity << "-qty " << temp->price << "Rs." << endl ;
        temp = temp->next;
    }
    file.close();
    cout << "Cart printed successfully!\n";
}

// Main Menu
// Main Menu
int main() {
    initializeHashTable();
    
    int choice, id, quantity;
    do {
        cout << "\n======================================\n";
        cout << "     Shopping Cart Manager       \n";
        cout << "======================================\n";
        cout << " [1]  Add to Cart\n";
        cout << " [2]  View Cart\n";
        cout << " [3]  Remove from Cart\n";
        cout << " [4]  View Inventory (Sorted)\n";
        cout << " [5]  Print Cart to File\n";
        cout << " [6]  Exit\n";
        cout << "--------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nEnter Product ID: ";
                cin >> id;
                cout << "Enter Quantity: ";
                cin >> quantity;
                addToCart(id, quantity);
                break;
            case 2:
                viewCart();
                break;
            case 3:
                cout << "\nEnter Product ID to Remove: ";
                cin >> id;
                removeFromCart(id);
                break;
            case 4:
                viewSortedInventory();
                break;
            case 5:
                printCart();
                break;
            case 6:
                cout << "\nExiting... Thank you for shopping!\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

