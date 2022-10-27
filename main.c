// header file include
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <struct.h>
#include <functions.h>

#include <termios.h>

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

// globarl Constant
int MAX_LEN = 25;
int MaxSize = 1500;
// global variable
int customerLimit;
int floorLimits;
int roomAtEachFloor;
int tax;
int days;

// global variable to take the upper limit reference
RoomType *roomTypeArr[3] = {0};
RoomDetail *roomsArr[500] = {0};
CustomerDetail *customersArr[1500] = {0};
BookingDetail *bookingsArr[1500] = {0};
BillBookDetail *billBookArr[1500] = {0};
CheckInOutDetail *checkInOutArr[1500] = {0};

// GLOBAL VARIABLE INITIALISATION
void login();
void init()
{
	login();
	roomTypeArr[0] = Object_new(2500, "Standard");
	roomTypeArr[1] = Object_new(3500, "Deluxe");
	roomTypeArr[2] = Object_new(5500, "Cottage");

	readMetaData();
	readRooms();
	readCustomers();
	readBills();
	readBookings();
	readChecks();

	customerLimit = floorLimits * roomAtEachFloor;
}

/*
 * FILE READ & WRITE FUNCTIONS (DATABASE READ & UPDATION)
 */
void readMetaData()
{
	FILE *fp;
	fp = fopen("../data/limitData.text", "r");

	if (fp != NULL)
	{
		char line[100];
		int k = 0;
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			char *p = strtok(line, ":");
			p = strtok(NULL, ":");
			if (k == 0)
			{
				sscanf(p, "%d", &floorLimits);
			}
			else if (k == 1)
			{
				sscanf(p, "%d", &roomAtEachFloor);
			}
			else if (k == 2)
			{
				sscanf(p, "%d", &tax);
			}
			else if (k == 3)
			{
				sscanf(p, "%d", &customersId);
			}
			else
			{
				sscanf(p, "%d", &bookingId);
			}
			p = strtok(NULL, ":");
			k++;
		}
	}
	fclose(fp);
}
void updateMetaData()
{
	FILE *fp;
	fp = fopen("../data/limitData.text", "w");

	fprintf(fp, "%s:%d\n", "floorLimits", floorLimits);
	fprintf(fp, "%s:%d\n", "roomAtEachFloor", roomAtEachFloor);
	fprintf(fp, "%s:%d\n", "tax", tax);
	fprintf(fp, "%s:%d\n", "customersId", customersId);
	fprintf(fp, "%s:%d\n", "bookingId", bookingId);

	fclose(fp);
}

void readCustomers()
{
	FILE *fp;

	fp = fopen("../data/Customers.text", "r");

	if (fp != NULL)
	{
		char line[100];
		int k = 0;
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			int customer_id;
			char *p = strtok(line, ":");
			CustomerDetail *obj = malloc(sizeof(CustomerDetail));
			while (p)
			{

				sscanf(p, "%d", &obj->customer_id);
				p = strtok(NULL, ":");

				obj->firstName = malloc(strlen(p) + 1);
				strcpy(obj->firstName, p);
				p = strtok(NULL, ":");

				obj->lastName = malloc(strlen(p) + 1);
				strcpy(obj->lastName, p);
				p = strtok(NULL, ":");

				obj->address = malloc(strlen(p) + 1);
				strcpy(obj->address, p);
				p = strtok(NULL, ":");
			}
			customersArr[k] = obj;
			k++;
		}
	}
	fclose(fp);
}
void updateCustomers()
{
	FILE *fp;
	fp = fopen("../data/Customers.text", "w");

	for (int i = 0; i < MaxSize; i++)
	{
		if (customersArr[i] != NULL)
		{
			fprintf(fp, "%d:%s:%s:%s", customersArr[i]->customer_id, customersArr[i]->firstName, customersArr[i]->lastName, customersArr[i]->address);
		}
	}

	fclose(fp);
}

void readRooms()
{
	FILE *fp;

	fp = fopen("../data/Rooms.text", "r");

	if (fp != NULL)
	{
		char line[100];
		int k = 0;
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			int customer_id;
			char *p = strtok(line, ":");
			RoomDetail *obj = malloc(sizeof(RoomDetail));
			while (p)
			{

				sscanf(p, "%d", &obj->room_no);
				p = strtok(NULL, ":");

				sscanf(p, "%d", &obj->floor_no);
				p = strtok(NULL, ":");

				sscanf(p, "%d", &obj->isAc);
				p = strtok(NULL, ":");

				sscanf(p, "%d", &obj->extra_bed);
				p = strtok(NULL, ":");

				obj->room_type = malloc(strlen(p) + 1);
				strcpy(obj->room_type, p);
				p = strtok(NULL, ":");

				obj->description = malloc(strlen(p) + 1);
				strcpy(obj->description, p);
				p = strtok(NULL, ":");
			}
			roomsArr[k] = obj;
			k++;
		}
	}
	fclose(fp);
}
void updateRooms()
{
	FILE *fp;
	fp = fopen("../data/Rooms.text", "w");

	for (int i = 0; i < 50; i++)
	{
		if (roomsArr[i] != NULL)
		{
			fprintf(fp, "%d:%d:%d:%d:%s:%s", roomsArr[i]->room_no, roomsArr[i]->floor_no, roomsArr[i]->isAc, roomsArr[i]->extra_bed, roomsArr[i]->room_type, roomsArr[i]->description);
		}
	}

	fclose(fp);
}

void readBookings()
{
	FILE *fp;

	fp = fopen("../data/Bookings.text", "r");

	if (fp != NULL)
	{
		char line[100];
		int k = 0;
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			int customer_id;
			char *p = strtok(line, ":");
			BookingDetail *obj = malloc(sizeof(BookingDetail));
			while (p)
			{

				sscanf(p, "%d", &obj->booking_id);
				p = strtok(NULL, ":");

				sscanf(p, "%d", &obj->customer_id);
				p = strtok(NULL, ":");

				sscanf(p, "%d", &obj->room_id);
				p = strtok(NULL, ":");

				sscanf(p, "%d", &obj->no_of_person);
				p = strtok(NULL, ":");

				obj->booking_date = malloc(strlen(p) + 1);
				strcpy(obj->booking_date, p);
				p = strtok(NULL, ":");

				obj->stay_start_date = malloc(strlen(p) + 1);
				strcpy(obj->stay_start_date, p);
				p = strtok(NULL, ":");

				obj->stay_end_date = malloc(strlen(p) + 1);
				strcpy(obj->stay_end_date, p);
				p = strtok(NULL, ":");
			}
			bookingsArr[k] = obj;
			k++;
		}
	}
	fclose(fp);
}
void updateBookings()
{
	FILE *fp;
	fp = fopen("../data/Bookings.text", "w");

	for (int i = 0; i < MaxSize; i++)
	{
		if (bookingsArr[i] != NULL)
		{
			fprintf(fp, "%d:%d:%d:%d:%s:%s:%s", bookingsArr[i]->booking_id, bookingsArr[i]->customer_id,bookingsArr[i]->room_id , bookingsArr[i]->no_of_person, bookingsArr[i]->booking_date, bookingsArr[i]->stay_start_date, bookingsArr[i]->stay_end_date);
		}
	}

	fclose(fp);
}

void readBills()
{
	FILE *fp;

	fp = fopen("../data/Bill-book.text", "r");

	if (fp != NULL)
	{
		char line[100];
		int k = 0;
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			int customer_id;
			char *p = strtok(line, ":");
			BillBookDetail *obj = malloc(sizeof(BillBookDetail));
			while (p)
			{

				sscanf(p, "%d", &obj->id);
				p = strtok(NULL, ":");

				sscanf(p, "%d", &obj->no_of_days);
				p = strtok(NULL, ":");

				sscanf(p, "%2f", &obj->total_bill);
				p = strtok(NULL, ":");

				obj->customer_name = malloc(strlen(p) + 1);
				strcpy(obj->customer_name, p);
				p = strtok(NULL, ":");
			}
			billBookArr[k] = obj;
			k++;
		}
	}
	fclose(fp);
}
void updateBills()
{
	FILE *fp;
	fp = fopen("../data/Bill-book.text", "w");

	for (int i = 0; i < MaxSize; i++)
	{
		if (billBookArr[i] != NULL)
		{
			fprintf(fp, "%d:%d:%2f:%s", billBookArr[i]->id, billBookArr[i]->no_of_days, billBookArr[i]->total_bill, billBookArr[i]->customer_name);
		}
	}

	fclose(fp);
}

void readChecks()
{
	FILE *fp;

	fp = fopen("../data/Check-in-out.text", "r");

	if (fp != NULL)
	{
		char line[100];
		int k = 0;
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			int customer_id;
			char *p = strtok(line, ":");
			CheckInOutDetail *obj = malloc(sizeof(CheckInOutDetail));
			while (p)
			{
				sscanf(p, "%d", &obj->id);
				p = strtok(NULL, ":");

				obj->status = malloc(strlen(p) + 1);
				strcpy(obj->status, p);
				p = strtok(NULL, ":");
			}
			checkInOutArr[k] = obj;
			k++;
		}
	}
	fclose(fp);
}
void updateChecks()
{
	FILE *fp;
	fp = fopen("../data/Check-in-out.text", "w");

	for (int i = 0; i < MaxSize; i++)
	{
		if (checkInOutArr[i] != NULL)
		{
			fprintf(fp, "%d:%s", checkInOutArr[i]->id, checkInOutArr[i]->status);
		}
	}

	fclose(fp);
}



/*
** HELPER FUNCTIONS
*/
// function to calculate the total_bill
float calculateTotalBill(int noOfDays, char *roomType, int food_charge, int extraperson_charge, int noOfExtraPerson)
{
	int fare = 0;
	for (int i = 0; i < 3; i++)
	{
		if (strcmp(roomTypeArr[i]->type, roomType) == 0)
		{
			fare = roomTypeArr[i]->fare;
		}
	}

	float total_bill = noOfDays * fare + noOfDays * food_charge + extraperson_charge * noOfExtraPerson;
	total_bill += (total_bill * tax) / 100;
	return total_bill;
}

// function to validate date
int isValidDate(int dd, int mm, int yy)
{
	if (yy >= 1900 && yy <= 9999)
	{
		// check month
		if (mm >= 1 && mm <= 12)
		{
			// check days
			if ((dd >= 1 && dd <= 31) && (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12))
				return 1;
			else if ((dd >= 1 && dd <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11))
				return 1;
			else if ((dd >= 1 && dd <= 28) && (mm == 2))
				return 1;
			else if (dd == 29 && mm == 2 && (yy % 400 == 0 || (yy % 4 == 0 && yy % 100 != 0)))
				return 1;
			else
				return 0;
		}
		return 0;
	}
	return 0;
}

int main()
{
	init();
	int i;

	time_t t;
	time(&t);
	char customername;
	char choice;
	system("clear");
	while (1)
	{
		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("   ******************************  |MAIN MENU|  ***************************** \n");
		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("\t\t *Please enter your choice for menu*:");
		printf("\n\n");
		printf(" \n Enter 1 -> Customer Edit");
		printf("\n********************************");
		printf(" \n Enter 2 -> Room Available");
		printf("\n********************************");
		printf(" \n Enter 3 -> Room Booking Options");
		printf("\n********************************");
		printf(" \n Enter 4 -> Generate Bill");
		printf("\n********************************");
		printf(" \n Enter 5 -> Check-In-Out Options");
		printf("\n********************************");
		printf(" \n Enter 6 -> Exit");
		printf("\n********************************");
		printf("\n");
		for (i = 0; i < 20; i++)
			printf("*");
		printf("\nCurrent date and time : %s", ctime(&t));
		for (i = 0; i < 20; i++)
			printf("*");

		choice = getche();
		choice = toupper(choice);
		switch (choice) // SWITCH STATEMENT
		{

		case '1':
			system("clear");
			customerEdit();
			break;
		case '2':
			system("clear");
			showRoomAvailable();
			break;
		case '3':
			system("clear");
			BookingsEdit();
			break;
		case '4':
			system("clear");
			generateBill();
			break;
		case '5':
			system("clear");
			CheckInOutEdit();
			break;
		case '6':

			printf("\n\n\t *****THANK YOU*****");
			printf("\n\t FOR TRUSTING OUR SERVICE");
			//	Sleep(2000);
			exit(0);
			break;
		default:

			//printf("Incorrect Input");
			//printf("\n Press any key to continue");
			system("clear");
			printf("Login Successfull!!\nPress any key to continue"); 
			getch();

		}
	}
}



/*
 * CUSTOMER DATABASE QUERY FUNCTIONS
*/

// Customer Database Edit
void customerEdit()
{

	int i = 0;
	char test, choice;

	while (1)
	{

		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("   ******************************  |MAIN MENU|  ***************************** \n");
		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("\t\t *Please enter your choice for menu*:");
		printf("\n\n");
		printf(" \n Enter 1 -> Add Customer Record");
		printf("\n ********************************");
		printf(" \n Enter 2 -> Update Customer Record");
		printf("\n ********************************");
		printf(" \n Enter 3 -> Delete Customer Record");
		printf("\n ********************************");
		printf(" \n Enter 4 -> main menu // login menu");
		printf("\n ********************************");
		printf(" \n Enter 5 -> Exit");
		printf("\n ********************************");
		printf("\n");
		for (i = 0; i < 20; i++)
			printf("*");
		for (i = 0; i < 20; i++)
			printf("*");

		choice = getche();
		choice = toupper(choice);
		switch (choice) // SWITCH STATEMENT
		{
		case '1':
			system("clear");
			customerAdd();
			break;
		case '2':
			system("clear");
			customerUpdate();
			break;
		case '3':
			system("clear");
			customerDelete();
			break;
		case '4' :
			system("clear");
			main();
			break;
		case '5':
			printf("\n\n\t *****THANK YOU*****");
			exit(0);
			break;
		default:
			printf("Incorrect Input");
			printf("\n Press any key to continue");
			getch();
		}
	}
}

// add new customer
void customerAdd()
{
	char *firstName = malloc(sizeof(char) * MAX_LEN);
	char *lastName = malloc(sizeof(char) * MAX_LEN);
	char *address = malloc(sizeof(char) * MAX_LEN);

	printf("\n Enter Customer Details:");
	printf("\n**************************\n");

	printf("Enter First Name: ");
	fgets(firstName, MAX_LEN, stdin);
	firstName[strcspn(firstName, "\r\n")] = 0;

	printf("Enter Last Name: ");
	fgets(lastName, MAX_LEN, stdin);
	lastName[strcspn(lastName, "\r\n")] = 0;

	printf("Enter Address: ");
	fgets(address, MAX_LEN, stdin);
	address[strcspn(address, "\r\n")] = 0;

	CustomerDetail *obj = Object_new_customer(firstName, lastName, address);
	if (strlen(firstName) > 0 && strlen(lastName) > 0 && strlen(address) > 0){
		
		FILE *fp;
		fp = fopen("../data/Customers.text", "a");
		fprintf(fp, "\n%d:%s:%s:%s", obj->customer_id, obj->firstName, obj->lastName, obj->address);
		fclose(fp);

		printf("Customer is successfully Registered with Id: %d", obj->customer_id);
		free(obj);
		
		printf("\n********************\n");
		printf("Press any key for continue");
		getch();
	}else{
		printf("Enter some data, empty string not allowed");
	}
}

// update customer
void customerUpdate()
{
	readCustomers();
	int cust_id, cust_idx = -1;
	printf("\nEnter the 'id' of customer to edit information: ");
	scanf("%d%c", &cust_id);
	for(int i = 0; i < MaxSize; i++){
		if(customersArr[i] == NULL) break;
		if(customersArr[i]->customer_id == cust_id){
			cust_idx = i;
			break;
		}
	}
	if(cust_idx == -1){
		printf("\nThere is no one customer with id: %d\n", cust_id);
		printf("Please Enter Again\n");
		return;
	}

	printf("\nPrevious Data\n");
	printf("\tFirst Name : %s\n\tLast Name: %s\n\tAddress: %s\n", customersArr[cust_idx]->firstName, customersArr[cust_idx]->lastName, customersArr[cust_idx]->address);

	char *firstName = malloc(sizeof(char) * MAX_LEN);
	char *lastName = malloc(sizeof(char) * MAX_LEN);
	char *address = malloc(sizeof(char) * MAX_LEN);

	printf("Enter New Detail for update previous:\n(Enter Some data if want to change)\n(Press Enter if don't want to change particular data in record)\n");
	printf("\n********************\n");

	printf("Enter First Name: ");
	fgets(firstName, MAX_LEN, stdin);
	firstName[strcspn(firstName, "\r\n")] = 0;

	printf("Enter Last Name: ");
	fgets(lastName, MAX_LEN, stdin);
	lastName[strcspn(lastName, "\r\n")] = 0;

	printf("Enter Address: ");
	fgets(address, MAX_LEN, stdin);
	address[strcspn(address, "\r")] = 0;
	
	if(strlen(firstName) > 0){
		customersArr[cust_idx]->firstName = firstName;
	}
	if(strlen(lastName) > 0){
		customersArr[cust_idx]->lastName = lastName;
	}
	if(strlen(address) > 1){
		customersArr[cust_idx]->address = address;
	}
	if(strlen(firstName) > 0 || strlen(lastName) > 0 || strlen(address) > 1){
		updateCustomers();
	}
	printf("Customer Data succesfully updated!!\n\n");
};

// delete customer
void customerDelete(){
	readCustomers();
	int cust_id, cust_idx = -1;
	printf("\nEnter the 'id' of customer to edit information: ");
	scanf("%d", &cust_id);
	for(int i = 0; i < MaxSize; i++){
		if(customersArr[i] == NULL) break;
		if(customersArr[i]->customer_id == cust_id){
			cust_idx = i;
			break;
		}
	}
	if(cust_idx == -1){
		printf("\nThere is no one customer with id: %d\n", cust_id);
		printf("Please Enter Again\n");
		return;
	}
	
	for(int i = cust_idx; i < MaxSize-1; i++){
		if(customersArr[i] == NULL) break;
		customersArr[i] = customersArr[i+1];
	}
	customersArr[MaxSize-1] = NULL;
	updateCustomers();

	printf("Customer Data succesfully Deleted!!\n\n");
};





/*
 * BOOKING DATABASE QUERY FUNCTIONS
*/

// bookings Database Edit
void BookingsEdit()
{

	int i = 0;
	char test, choice;

	while (1)
	{

		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("   ******************************  |MAIN MENU|  ***************************** \n");
		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("\t\t *Please enter your choice for menu*:");
		printf("\n\n");
		printf(" \n Enter 1 -> Book New Room");
		printf("\n ********************************");
		printf(" \n Enter 2 -> Update Booking Record");
		printf("\n ********************************");
		printf(" \n Enter 3 -> Delete Booking Record");
		printf("\n ********************************");
		printf(" \n Enter 4 -> main menu // login menu ");
		printf("\n ********************************");
		printf(" \n Enter 5 -> Exit");
		printf("\n ********************************");
		printf("\n");
		for (i = 0; i < 20; i++)
			printf("*");
		for (i = 0; i < 20; i++)
			printf("*");

		choice = getche();
		choice = toupper(choice);
		switch (choice) // SWITCH STATEMENT
		{
		case '1':
			system("clear");
			BookingsAdd();
			break;
		case '2':
			system("clear");
			BookingsUpdate();
			break;
		case '3':
			system("clear");
			BookingsDelete();
			break;
		case '4' :
			system("clear");
			main();
			break;
		case '5':
			printf("\n\n\t *****THANK YOU*****");
			//	Sleep(2000);
			exit(0);
			break;
		default:
			printf("Incorrect Input");
			printf("\n Press any key to continue");
			getch();
		}
	}
}

// book new room
void BookingsAdd()
{

	int valid = 0;
	int customer_id, room_id, persons;

	printf("\n Enter Room Details For book:");
	printf("\n**************************\n");

	printf("\nEnter Customer Id: ");
	scanf("%d",&customer_id);
	readCustomers();
	for(int i = 0; i < MaxSize; i++){
		if(customersArr[i] == NULL) break;
		if(customersArr[i]->customer_id == customer_id){
			valid = 1;
			break;
		}
	}
	if(valid == 0){
		printf("\nCustomer is not present in database with given id\n");
		printf("Please Try Again\n");
		return;
	}

	
	printf("Below are available rooms with id\n");
	showRoomAvailable();

	printf("\nEnter Room Id: ");
	scanf("%d",&room_id);

	for(int i = 0; i < MaxSize; i++){
		if(bookingsArr[i] == NULL) break;
		if(bookingsArr[i]->room_id == room_id){
			valid = 0;
			break;
		}
	}
	if(valid == 0){
		printf("Room is already Booked\n");
		printf("Please try again\n");
		return;
	}

	printf("\n Enter number of days: ");
	scanf("%d",&days);
	printf("\nEnter No of person: ");
	scanf("%d",&persons);
	
	if(persons > 3){
		printf("\nOnly 2 extra person is allowed only\n");
		printf("Please Try Again\n");
		return;
	}



	char *bookingDate = malloc(sizeof(char) * 11);
	char *StartDate = malloc(sizeof(char) * 11);
	char *EndDate = malloc(sizeof(char) * 11);

	int day, month, year;
	printf("Enter Booking Date\n");

	printf("Enter day: ");
	scanf("%d", &day);
	printf("\nEnter Month: ");
	scanf("%d", &month);
	printf("\nEnter Year: ");
	scanf("%d", &year);

	if(isValidDate(day, month, year) == 0){
		printf("\nBooking Date is not valid, Please try again\n");
		return;
	}

	char dr[3],mr[3];
	if(day < 10){
		sprintf(dr, "0%d", day);
	}else{
		sprintf(dr, "%d", day);
	}

	if(month < 10){
		sprintf(mr, "0%d", month);
	}else{
		sprintf(mr, "%d", month);
	}

	sprintf(bookingDate, "%s-%s-%d", dr, mr, year);

//--------------------------------------
	printf("Enter Start Date\n");

	printf("Enter day: ");
	scanf("%d", &day);
	printf("\nEnter Month: ");
	scanf("%d", &month);
	printf("\nEnter Year: ");
	scanf("%d", &year);

	if(isValidDate(day, month, year) == 0){
		printf("\nStart Date is not valid, Please try again\n");
		return;
	}

	if(day < 10){
		sprintf(dr, "0%d", day);
	}else{
		sprintf(dr, "%d", day);
	}

	if(month < 10){
		sprintf(mr, "0%d", month);
	}else{
		sprintf(mr, "%d", month);
	}

	sprintf(StartDate, "%s-%s-%d", dr, mr, year);

//----------------------------------------
	printf("Enter End Date\n");

	printf("Enter day: ");
	scanf("%d", &day);
	printf("\nEnter Month: ");
	scanf("%d", &month);
	printf("\nEnter Year: ");
	scanf("%d", &year);

	if(isValidDate(day, month, year) == 0){
		printf("\nEnd Date is not valid, Please try again\n");
		return;
	}

	if(day < 10){
		sprintf(dr, "0%d", day);
	}else{
		sprintf(dr, "%d", day);
	}

	if(month < 10){
		sprintf(mr, "0%d", month);
	}else{
		sprintf(mr, "%d", month);
	}

	sprintf(EndDate, "%s-%s-%d", dr, mr, year);


	BookingDetail *obj = Object_new_booking(customer_id, room_id, persons, bookingDate, StartDate, EndDate);
	FILE *fp;
	fp = fopen("../data/Bookings.text", "a");
	fprintf(fp, "%d:%d:%d:%d:%s:%s:%s\n", obj->booking_id, obj->customer_id,obj->room_id , obj->no_of_person, obj->booking_date, obj->stay_start_date, obj->stay_end_date);
	fclose(fp);

	printf("Rooms is successfully Booked with Customer: %d . Your booking id is %d ", obj->customer_id, obj->booking_id);
	free(obj);
	
	printf("\n*************\n");
	printf("Press any key for continue");
	getch();
}

//booked update
void BookingsUpdate(){
	
	readBookings();
	int booking_id, booking_idx = -1;
	printf("\nEnter the 'id' of booking to delete information: ");
	scanf("%d", &booking_id);
	for(int i = 0; i < MaxSize; i++){
		if(bookingsArr[i] == NULL) break;
		if(bookingsArr[i]->booking_id == booking_id){
			booking_idx = i;
			break;
		}
	}
	if(booking_idx == -1){
		printf("\nThere is no one Booking with id: %d\n", booking_id);
		printf("Please Enter Again\n");
		return;
	}

	printf("Booking Record succesfully Updated!!\n\n");
}

// delete bookings
void BookingsDelete(){
	readBookings();
	int booking_id, booking_idx = -1;
	printf("\nEnter the 'id' of booking to delete information: ");
	scanf("%d", &booking_id);
	for(int i = 0; i < MaxSize; i++){
		if(bookingsArr[i] == NULL) break;
		if(bookingsArr[i]->booking_id == booking_id){
			booking_idx = i;
			break;
		}
	}
	if(booking_idx == -1){
		printf("\nThere is no one Booking with id: %d\n", booking_id);
		printf("Please Enter Again\n");
		return;
	}
	
	for(int i = booking_idx; i < MaxSize-1; i++){
		if(bookingsArr[i] == NULL) break;
		bookingsArr[i] = bookingsArr[i+1];
	}
	bookingsArr[MaxSize-1] = NULL;
	updateBookings();

	printf("Booking Record succesfully Deleted!!\n\n");
};





/*
 * CHECK-IN-OUT DATABASE QUERY FUNCTIONS
*/
// CheckInOut Database Edit
void CheckInOutEdit()
{

	int i = 0;
	char test, choice;

	while (1)
	{

		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("   ******************************  |MAIN MENU|  ***************************** \n");
		for (i = 0; i < 20; i++)
			printf("*");
		printf("\n");
		printf("\t\t *Please enter your choice for menu*:");
		printf("\n\n");
		printf(" \n Enter 1 -> Add CHeckInOut Record");
		printf("\n ********************************");
		printf(" \n Enter 2 -> Delete CHeckInOut Record");
		printf("\n ********************************");
		printf(" \n Enter 3 -> main menu // login menu ");
		printf("\n ********************************");
		printf(" \n Enter 4 -> Exit");
		printf("\n ********************************");
		printf("\n");
		for (i = 0; i < 20; i++)
			printf("*");
		for (i = 0; i < 20; i++)
			printf("*");

		choice = getche();
		choice = toupper(choice);
		switch (choice) // SWITCH STATEMENT
		{
		case '1':
			system("clear");
			CheckInOutAdd();
			break;
		case '2':
			system("clear");
			CheckInOutDelete();
			break;
		case '3' :
			system("clear");
			main();
			break;
		case '4':
			printf("\n\n\t *****THANK YOU*****");
			//	Sleep(2000);
			exit(0);
			break;
		default:
			printf("Incorrect Input");
			printf("\n Press any key to continue");
			getch();
		}
	}
}

// add new CheckInOut
void CheckInOutAdd()
{
	readBookings();
	int booking_id, booking_idx = -1;
	printf("\nEnter the 'id' of booking to add information in Check-In-Out: ");
	scanf("%d", &booking_id);
	for(int i = 0; i < MaxSize; i++){
		if(bookingsArr[i] == NULL) break;
		if(bookingsArr[i]->booking_id == booking_id){
			booking_idx = i;
			break;
		}
	}
	if(booking_idx == -1){
		printf("\nThere is no one Booking with id: %d\n", booking_id);
		printf("Please Enter Again\n");
		return;
	}
	
	printf("\nEnter selection\nFor Check In -> 1\nFor Check Out -> 2\n");
	int slect;
	scanf("%d", &slect);
	if(slect == 1){
		CheckInOutDetail *obj = Object_new_check(booking_id, "Check-In");
		FILE *fp;
		fp = fopen("../data/Check-in-out.text", "a");
		fprintf(fp, "%d:%s\n", obj->id, obj->status);
		fclose(fp);

		printf("Check-In Out is successfully added with booking Id: %d", obj->id);
		free(obj);
		
		printf("\n**************************\n");
		printf("Press any key for continue");
		getch();
	}else if(slect == 2){
		CheckInOutDetail *obj = Object_new_check(booking_id, "Check-Out");
		FILE *fp;
		fp = fopen("../data/Check-in-out.text", "a");
		fprintf(fp, "%d:%s\n", obj->id, obj->status);
		fclose(fp);

		printf("Check-In Out is successfully added with booking Id: %d", obj->id);
		free(obj);
		
		printf("\n**************************\n");
		printf("Press any key for continue");
		getch();
	}else{
		printf("False Selection! Try Again\n");
	}

}

// delete CheckInOut
void CheckInOutDelete(){
	readChecks();
	int booking_id, booking_idx = -1;
	printf("\nEnter the booking id to delete information in Check-In-Out: ");
	scanf("%d", &booking_id);
	printf("%d", booking_id);
	for(int i = 0; i < MaxSize; i++){
		printf("%d", checkInOutArr[i]->id);
		if(checkInOutArr[i] == NULL) break;
		if(checkInOutArr[i]->id == booking_id){
			booking_idx = i;
			break;
		}
	}
	if(booking_idx == -1){
		printf("\nThere is no Check-In-Out with id: %d\n", booking_id);
		printf("Please Enter Again\n");
		return;
	}

	for(int i = booking_idx; i < MaxSize-1; i++){
		if(checkInOutArr[i] == NULL) break;
		checkInOutArr[i] = checkInOutArr[i+1];
	}
	updateChecks();

	printf("Check In Out record succesfully deleted");
	
};


/*
*	MENU FUNCTIONS
*/
// show which rooms are available
void showRoomAvailable(){
	readBookings();
	printf("\nThese are the room which is currentely available\n[");
	for(int i = 0; i < floorLimits*roomAtEachFloor; i++){
		int ok = 1;
		for(int j = 0; j < MaxSize; j++){
			if(bookingsArr[j] == NULL) break;
			if(bookingsArr[j]->room_id == i+1){
				ok = 0;
			}
		}
		if(ok){
			printf("%d,",i+1);
		}
	}
	printf("]\n");
};
// generate bill
void generateBill(){
	readBookings();
	readBills();

	int booking_id, booking_idx = -1;
	printf("\nEnter the booking id to generate the bill: ");
	scanf("%d", &booking_id);
	for(int i = 0; i < MaxSize; i++){
		if(bookingsArr[i] == NULL) break;
		if(bookingsArr[i]->booking_id == booking_id){
			booking_idx = i;
			break;
		}
	}
	if(booking_idx == -1){
		printf("\nThere is no one Booking with id: %d\n", booking_id);
		printf("Please Enter Again\n");
		return;
	}
	
	int cust_id = bookingsArr[booking_idx]->customer_id;
	int room_id = bookingsArr[booking_idx]->room_id;
	int extraperson = bookingsArr[booking_idx]->no_of_person-1;

	char cust_name[100], rt[100];
	
	
	readCustomers();
	for(int i = 0; i < MaxSize; i++){
		if(customersArr[i] == NULL) break;
		if(customersArr[i]->customer_id == cust_id){
			strcpy(cust_name, customersArr[i]->firstName);
			break;
		}
	}

	strcpy(rt, roomsArr[room_id-1]->room_type);

	//int noOfDays, char *roomType, int food_charge, int extraperson_charge, int noOfExtraPerson
	//int days = 10;
	int foodChrge = days*100;
	int extrapersonCharge = 200;
	float totalBill = calculateTotalBill(days, rt, foodChrge, extrapersonCharge,extraperson);

	int bill_id = 1;
	int i = 0;
	for(i = 0; i < MaxSize; i++){
		if(billBookArr[i] == NULL) break;
		bill_id = billBookArr[i]->id+1;
	}

	BillBookDetail* obj = Object_new_bill(bill_id, days, totalBill, cust_name);
	FILE *fp;
	fp = fopen("../data/Bill-book.text", "a");
	fprintf(fp, "%d:%d:%2f:%s\n", obj->id, obj->no_of_days, obj->total_bill, obj->customer_name);
	fclose(fp);

	printf("\nThe Bill is : ");
	printf("\nbill id = %d\nNo Of Days(Stay) = %d\nTotal Bill amount = %2f\nCustomer Name = %s\n", obj->id, obj->no_of_days, obj->total_bill, obj->customer_name);
	printf("\nBill Succesfully Generated!!\n");
	free(obj);
	printf("Bill Record succesfully Generated!!\n\n");
	getch();
	printf("\n**************************\n");
	printf("Press any key for continue");
	
	
};

// dummy function
void generateRoomsData()
{
	int k = 0;
	for (int i = 0; i < floorLimits; i++)
	{
		for (int j = 0; j < roomAtEachFloor; j++)
		{
			int x = rand();
			char *t = malloc(strlen(roomTypeArr[x % 3]->type) + 1);
			strcpy(t, roomTypeArr[x % 3]->type);
			roomsArr[k] = Object_new_room(j + 1, i + 1, x % 2, x % 5, t, "description is not important\n");
			k++;
		}
	}
	updateRooms();
}

