#include "Test.h"

#include "Activity.h"
#include "ActivityValidator.h"
#include "StringParser.h"
#include "ValidationException.h"
#include "ActivityRepositoryInMemory.h"
#include "RepositoryException.h"
#include "ActivityRepositoryInFile.h"
#include "ActivityManagerService.h"
#include "ActivityManagerServiceWithWishlist.h"
#include "ControllerException.h"
#include "ActivityManagerServiceWithWishlistAndUndo.h"


#include <vector>
#include <string>
#include <assert.h>
#include <iostream>


void testStripSpaces()
{
	assert(stripSpaces("aaaaa    ") == "aaaaa");
	assert(stripSpaces("     aaaaa") == "aaaaa");
	assert(stripSpaces("aaaaa") == "aaaaa");
	assert(stripSpaces("aa aa") == "aa aa");
	assert(stripSpaces("     ") == "");
	assert(stripSpaces("") == "");
}

void testUtils()
{
	testStripSpaces();
}

void testDomainActivity()
{
	Activity someActivity{ "fishing", "trip", "outdoor", 600 };

	assert(someActivity.getActivityName() == "fishing");
	assert(someActivity.getActivityDescription() == "trip");
	assert(someActivity.getActivityType() == "outdoor");
	assert(someActivity.getActivityDuration() == 600);
}

void testDomainActivityValidator()
{
	ActivityValidator validator;

	validator.validateActivity({ "a", "a", "a",1 });

	try {
		validator.validateActivity({ "","","",-1 });assert(false);}
	catch (const ValidationException& exception) {
		assert(exception.getExceptionMessage() == "Invalid name!\nInvalid description!\nInvalid type!\nInvalid duration!\n");
	}

	try {
		validator.validateActivity({ "","a","",0 });assert(false);}
	catch (const ValidationException& exception) {
		assert(exception.getExceptionMessage() == "Invalid name!\nInvalid type!\nInvalid duration!\n");
	}

	
}

void testDomain()
{
	testDomainActivity();
	testDomainActivityValidator();
}


void tearDownRepository(ActivityRepositoryInMemory& testRepository) {
	while (testRepository.getNumberOfActivities() > 0) {
		const auto& activities = testRepository.getAllActivities();
		testRepository.removeActivityByName(activities.at(0).getActivityName());
	}

}
void setUpRepository(ActivityRepositoryInMemory& testRepository) {
	tearDownRepository(testRepository);
	testRepository.addActivity({ "fishing", "trip", "outdoor", 480 });
	testRepository.addActivity({ "kayaking", "trip", "outdoor", 300 });
	testRepository.addActivity({ "dance class", "course", "indoor", 200 });
	testRepository.addActivity({ "guitar class", "course", "indoor", 180 });
}


bool checkIfActivitiesAreIdentical(const Activity& thisActivity, const Activity& otherActivity) noexcept {
	bool flag = true;
	flag = flag & (thisActivity.getActivityName() == otherActivity.getActivityName());
	flag = flag & (thisActivity.getActivityDescription() == otherActivity.getActivityDescription());
	flag = flag & (thisActivity.getActivityType() == otherActivity.getActivityType());
	flag = flag & (thisActivity.getActivityDuration() == otherActivity.getActivityDuration());
	return flag;
}

void testAddActivityRepository(ActivityRepositoryInMemory& testRepository) {
	assert(testRepository.getNumberOfActivities() == 4);

	testRepository.addActivity({ "some","some","some",10 });
	const auto& activities = testRepository.getAllActivities();
	assert(checkIfActivitiesAreIdentical(activities.back(), { "some","some","some",10 }));
	try { testRepository.addActivity({ "some","some","some",10 }); assert(false); }
	catch (const RepositoryException& exception) { assert(exception.getExceptionMessage() == "Activity already exists!\n"); }
}


void testGetAllActivitiesRepository(ActivityRepositoryInMemory& testRepository) {
	assert(testRepository.getNumberOfActivities() == 4);
	const auto& activities = testRepository.getAllActivities();
	assert(checkIfActivitiesAreIdentical(activities.at(0), { "fishing" ,"trip","outdoor", 480 }));
	assert(checkIfActivitiesAreIdentical(activities.at(1), { "kayaking", "trip", "outdoor", 300 }));
	assert(checkIfActivitiesAreIdentical(activities.at(2), { "dance class", "course", "indoor", 200 }));
	assert(checkIfActivitiesAreIdentical(activities.at(3), { "guitar class", "course", "indoor", 180 }));
	tearDownRepository(testRepository);
	assert(testRepository.getAllActivities().size() == 0);
}

void testFindActivittByNameRepository(ActivityRepositoryInMemory& testRepository) {
	assert(testRepository.getNumberOfActivities() == 4);
	auto activity = testRepository.findActivityByName("dance class");
	assert(checkIfActivitiesAreIdentical(activity, { "dance class", "course", "indoor", 200 }));
	try { testRepository.findActivityByName("some"); assert(false); }
	catch (const RepositoryException& exception) { assert(exception.getExceptionMessage() == "Activity doesn't exist!\n"); }
}

void testRemoveActivityByNameRepository(ActivityRepositoryInMemory& testRepository) {
	assert(testRepository.getNumberOfActivities() == 4);
	testRepository.removeActivityByName("fishing");
	assert(testRepository.getNumberOfActivities() == 3);
	try { testRepository.findActivityByName("fishing"); assert(false); }
	catch (const RepositoryException& exception) { assert(exception.getExceptionMessage() == "Activity doesn't exist!\n"); }
	
	try { testRepository.removeActivityByName("some"); assert(false); }
	catch (const RepositoryException& exception) { assert(exception.getExceptionMessage() == "Activity doesn't exist!\n"); }
}

typedef void (*repositoryTestFunction)(ActivityRepositoryInMemory&);
void setUpTestReposotoryMethodAndTearDown(ActivityRepositoryInMemory& repository, repositoryTestFunction testFunction)
{
	setUpRepository(repository);
	if (testFunction)
		testFunction(repository);
	tearDownRepository(repository);
}

void testRepository(ActivityRepositoryInMemory& repository)
{
	setUpTestReposotoryMethodAndTearDown(repository, testAddActivityRepository);
	setUpTestReposotoryMethodAndTearDown(repository, testGetAllActivitiesRepository);
	setUpTestReposotoryMethodAndTearDown(repository, testFindActivittByNameRepository);
	setUpTestReposotoryMethodAndTearDown(repository, testRemoveActivityByNameRepository);
}

void createAndTestRepository() {
	ActivityRepositoryInMemory repository;
	testRepository(repository);
}

void createAndTestRepositoryWithFile(const std::string& fileName) {
	ActivityRepositoryInFile repository(fileName);
	testRepository(repository);
}


void tearDownService(ActivityManagerService& managerService)
{
	size_t numberOfActivities = managerService.getNumberOfActivitiesService();
	while (numberOfActivities > 0)
	{
		std::string name;
		const auto& activities = managerService.getAllActivitiesService();
		name = activities.at(0).getActivityName();
		managerService.removeActivityByNameService(name);
		numberOfActivities--;
	}
}

void testServiceAddActivity(ActivityManagerService& managerService)
{
	assert(managerService.getNumberOfActivitiesService() == 4);
	const auto& activities = managerService.getAllActivitiesService();
	assert(activities.size() == 4);
	try {managerService.addActivityService("", "", "a", 10); assert(false);}
	catch (const ValidationException& exception) {assert(exception.getExceptionMessage() == "Invalid name!\nInvalid description!\n");}
	
	try {managerService.addActivityService("fishing", "trip", "outdoor", 480);assert(false);}
	catch (const RepositoryException& exception){assert(exception.getExceptionMessage() == "Activity already exists!\n");}
}

void testServiceGetAllActivities(ActivityManagerService& managerService)
{	
	assert(managerService.getNumberOfActivitiesService() == 4);
	const auto& activities = managerService.getAllActivitiesService();
	assert(activities.size() == 4);
	assert(checkIfActivitiesAreIdentical(activities.at(0), { "fishing" ,"trip","outdoor", 480 }));
	assert(checkIfActivitiesAreIdentical(activities.at(1), { "kayaking", "trip", "outdoor", 300 }));
	assert(checkIfActivitiesAreIdentical(activities.at(2), { "dance class", "course", "indoor", 200 }));
	assert(checkIfActivitiesAreIdentical(activities.at(3), { "guitar class", "course", "indoor", 180 }));
	tearDownService(managerService);
	assert(managerService.getAllActivitiesService().size() == 0);
}

void testServiceRemoveActivityByName(ActivityManagerService& managerService)
{
	assert(managerService.getNumberOfActivitiesService() == 4);
	try {managerService.removeActivityByNameService("some name");assert(false);}
	catch (const RepositoryException& exception){assert(exception.getExceptionMessage() == "Activity doesn't exist!\n");}
	managerService.removeActivityByNameService("fishing");
	assert(managerService.getNumberOfActivitiesService() == 3);
}

void testServiceFindActivityByName(ActivityManagerService& managerService)
{
	assert(managerService.getNumberOfActivitiesService() == 4);
	const auto activity = managerService.findActivityByNameService("fishing");
	assert(activity.getActivityDescription() == "trip");
	assert(activity.getActivityType() == "outdoor");
	assert(activity.getActivityDuration() == 480);
	try {managerService.findActivityByNameService("some name");assert(false);}
	catch (const RepositoryException& exception) {assert(exception.getExceptionMessage() == "Activity doesn't exist!\n");}
}

void testServiceUpdateActivity(ActivityManagerService& managerService)
{
	assert(managerService.getNumberOfActivitiesService() == 4);
	managerService.updateActivityService("fishing", "some1", "some2", 10);
	const auto activity = managerService.findActivityByNameService("fishing");
	assert(activity.getActivityDescription() == "some1");
	assert(activity.getActivityType() == "some2");
	assert(activity.getActivityDuration() == 10);
	try {managerService.updateActivityService("some name", "sd", "asd", 10);assert(false);}
	catch (const RepositoryException& exception){assert(exception.getExceptionMessage() == "Activity doesn't exist!\n");}
	
	try {managerService.updateActivityService("", "", "", -1);assert(false);}
	catch (const ValidationException& exception) {assert(exception.getExceptionMessage() == "Invalid name!\nInvalid description!\nInvalid type!\nInvalid duration!\n");}
}

void testServiceFilterActivities(ActivityManagerService& managerService)
{
	// first we filter by description
	
	assert(managerService.getNumberOfActivitiesService() == 4);
	auto resultOfDescriptionFiltration = managerService.filterActivitiesByDescriptionService("trip");

	assert(resultOfDescriptionFiltration.size() == 2);
	assert(resultOfDescriptionFiltration.at(0).getActivityName() == "fishing");
	assert(resultOfDescriptionFiltration.at(1).getActivityName() == "kayaking");

	// now we filter by type
	auto resultOfTypeFiltration = managerService.filterActivitiesByTypeService("indoor");
	assert(resultOfTypeFiltration.size() == 2);
	assert(resultOfTypeFiltration.at(0).getActivityName() == "dance class");
	assert(resultOfTypeFiltration.at(1).getActivityName() == "guitar class");

	tearDownService(managerService);
}

void testServiceSortActivities(ActivityManagerService& managerService)
{
	assert(managerService.getNumberOfActivitiesService() == 4);
	//we sort after description
	auto activitiesSorted1 = managerService.sortActivitiesByDescriptionService();
	assert(activitiesSorted1.size() == 4);
	assert(activitiesSorted1.at(0).getActivityName() == "dance class");
	assert(activitiesSorted1.at(1).getActivityName() == "guitar class");
	assert(activitiesSorted1.at(2).getActivityName() == "fishing");
	assert(activitiesSorted1.at(3).getActivityName() == "kayaking");

	//we sort after name
	auto activitiesSorted2 = managerService.sortActivitiesByNameService();
	assert(activitiesSorted2.size() == 4);
	assert(activitiesSorted2.at(0).getActivityName() == "dance class");
	assert(activitiesSorted2.at(1).getActivityName() == "fishing");
	assert(activitiesSorted2.at(2).getActivityName() == "guitar class");
	assert(activitiesSorted2.at(3).getActivityName() == "kayaking");

	//we sort after type and duration
	auto activitiesSorted3 = managerService.sortActivitiesByTypeAndDurationService();
	assert(activitiesSorted3.at(1).getActivityName() == "dance class");
	assert(activitiesSorted3.at(3).getActivityName() == "fishing");
	assert(activitiesSorted3.at(0).getActivityName() == "guitar class");
	assert(activitiesSorted3.at(2).getActivityName() == "kayaking");

	managerService.removeActivityByNameService("dance class");
	managerService.removeActivityByNameService("fishing");
	managerService.removeActivityByNameService("guitar class");
	managerService.removeActivityByNameService("kayaking");
	
	managerService.addActivityService("dance class", "course", "indoor", 180);
	managerService.addActivityService("fishing", "trip", "outdoor", 300);
	managerService.addActivityService("guitar class", "course", "indoor", 200);
	managerService.addActivityService("kayaking", "trip", "outdoor", 480);

}

void setUpService(ActivityManagerService& managerService)
{
	tearDownService(managerService);
	managerService.addActivityService("fishing", "trip", "outdoor", 480);
	managerService.addActivityService("kayaking", "trip", "outdoor", 300);
	managerService.addActivityService("dance class", "course", "indoor", 200);
	managerService.addActivityService("guitar class", "course", "indoor", 180);

	assert(managerService.getNumberOfActivitiesService() == 4);
}

typedef void (*serviceTestFunction)(ActivityManagerService&);
void setUpTestServiceMethodAndTearDown(ActivityManagerService& managerService, serviceTestFunction testFunction) {
	setUpService(managerService);
	if (testFunction)
		testFunction(managerService);
	tearDownService(managerService);
}

void testService(ActivityManagerService& managerService)
{
	setUpTestServiceMethodAndTearDown(managerService, testServiceAddActivity);
	setUpTestServiceMethodAndTearDown(managerService, testServiceGetAllActivities);
	setUpTestServiceMethodAndTearDown(managerService, testServiceRemoveActivityByName);
	setUpTestServiceMethodAndTearDown(managerService, testServiceFindActivityByName);
	setUpTestServiceMethodAndTearDown(managerService, testServiceUpdateActivity);
	setUpTestServiceMethodAndTearDown(managerService, testServiceFilterActivities);
	setUpTestServiceMethodAndTearDown(managerService, testServiceSortActivities);
}

void createAndTestService()
{
	ActivityValidator validator;
	ActivityRepositoryInMemory repository;
	ActivityManagerService managerService(repository, validator);
	testService(managerService);
}

void createAndTestServiceWithFile(const std::string& testFileName)
{
	ActivityValidator validator;
	ActivityRepositoryInFile repository(testFileName);
	ActivityManagerService managerService(repository, validator);
	testService(managerService);
}



void testRemoveAllActivitiesFromWishlistService(ActivityManagerServiceWithWishlist& managerService) {
	assert(managerService.getWishlistSizeService() == 4);
	managerService.removeAllFromWishlistService();

	assert(managerService.getWishlistSizeService() == 0);
}
void testAddActivityToWishlistService(ActivityManagerServiceWithWishlist& managerService) {
	assert(managerService.getWishlistSizeService() == 4);
	managerService.addActivityToWishlistService("fishing");

	assert(managerService.getWishlistSizeService() == 5);

	assert(managerService.getWishlistActivitiesService().at(4).getActivityName() == "fishing");

	try { managerService.addActivityToWishlistService("some name"); assert(false); }
	catch (const RepositoryException& exception) { assert(exception.getExceptionMessage() == "Activity doesn't exist!\n"); }
}

void testGenerateRandomWishlistService(ActivityManagerServiceWithWishlist& managerService) {
	managerService.generateRandomWishlistService(2);
	assert(managerService.getWishlistSizeService() == 2);
	managerService.generateRandomWishlistService(5);
	assert(managerService.getWishlistSizeService() == 5);
}

#include <fstream>

std::vector<Activity> readWishlistFromFile(const std::string& fileName) {
	std::ifstream testFileInputStream;
	testFileInputStream.open(fileName);

	if (!testFileInputStream.is_open())
		return {};

	std::vector<Activity> activitiesFromFile;
	
	while (!testFileInputStream.eof())
	{
		std::string name, description, type, durationString;
		std::getline(testFileInputStream, name, ',');
		if (testFileInputStream.eof())
			break;
		std::getline(testFileInputStream, description, ',');
		std::getline(testFileInputStream, type, ',');
		std::getline(testFileInputStream, durationString, '\n');
		activitiesFromFile.emplace_back(name, description, type, std::stoi(durationString));
	
	}
	testFileInputStream.close();
	return activitiesFromFile;
}
void testExportWishlistToCsvFileService(ActivityManagerServiceWithWishlist& managerService) {
	assert(managerService.getWishlistSizeService() == 4);
	managerService.exportWishlistToCsvFileService("test_file_CSV.txt");
	auto activitiesFromFile = readWishlistFromFile("test_file_CSV.txt");
	assert(activitiesFromFile.size() == 4);
	const auto& activitiesFromWishlist = managerService.getWishlistActivitiesService();
	for (size_t i = 0; i < 4; i++)
		checkIfActivitiesAreIdentical(activitiesFromFile.at(i), activitiesFromWishlist.at(i));
}

void testGetWishlistService(ActivityManagerServiceWithWishlist& managerService) {
	assert(managerService.getWishlistSizeService() == 4);
	assert(managerService.getWishlistActivitiesService().at(0).getActivityName() == "fishing");
	assert(managerService.getWishlistActivitiesService().at(1).getActivityName() == "dance class");
	assert(managerService.getWishlistActivitiesService().at(2).getActivityName() == "guitar class");
	assert(managerService.getWishlistActivitiesService().at(3).getActivityName() == "kayaking");
	managerService.removeAllFromWishlistService();
	
}

void tearDownServiceWithWishlist(ActivityManagerServiceWithWishlist& managerService) {
	tearDownService(managerService);
	if(managerService.getWishlistSizeService() > 0)
		managerService.removeAllFromWishlistService();
}

void setUpServiceWithWishlist(ActivityManagerServiceWithWishlist& managerService) {
	tearDownServiceWithWishlist(managerService);
	setUpService(managerService);
	managerService.addActivityToWishlistService("fishing");
	managerService.addActivityToWishlistService("dance class");
	managerService.addActivityToWishlistService("guitar class");
	managerService.addActivityToWishlistService("kayaking");
}


typedef void (*serviceWithWishlistTestFunction)(ActivityManagerServiceWithWishlist&);
void setUpTestServiceWithWishListAndTearDown(ActivityManagerServiceWithWishlist& managerService, serviceWithWishlistTestFunction testFunction) {
	setUpServiceWithWishlist(managerService);
	if (testFunction)
		testFunction(managerService);
	tearDownServiceWithWishlist(managerService);
}

void testServiceWishlistMethods(ActivityManagerServiceWithWishlist& managerService) {
	setUpTestServiceWithWishListAndTearDown(managerService, testAddActivityToWishlistService);
	setUpTestServiceWithWishListAndTearDown(managerService, testRemoveAllActivitiesFromWishlistService);
	setUpTestServiceWithWishListAndTearDown(managerService, testExportWishlistToCsvFileService);
	setUpTestServiceWithWishListAndTearDown(managerService, testGetWishlistService);
	setUpTestServiceWithWishListAndTearDown(managerService, testGenerateRandomWishlistService);
}

void createAndTestServiceWithWishlist() {
	ActivityValidator validator;
	ActivityRepositoryInMemory repository;
	ActivityWishlist wishlist(repository);
	ActivityManagerServiceWithWishlist managerService(repository, validator, wishlist);
	testService(managerService);
	testServiceWishlistMethods(managerService);
}



void tearDownWishlist(ActivityWishlist& wishlist)
{
	if (wishlist.getWishlistSize())
		wishlist.removeAllActivitiesFromWishlist();
}

void setUpWishlist(ActivityWishlist& wishlist)
{
	tearDownWishlist(wishlist);
	wishlist.addActivityToWishlist("fishing");
	wishlist.addActivityToWishlist("dance class");
	wishlist.addActivityToWishlist("guitar class");
	wishlist.addActivityToWishlist("kayaking");
}

typedef void (*testWishlistFunction)(ActivityWishlist&);
void setUpTestWishlistAndTearDown(ActivityWishlist& wishlist, testWishlistFunction testFunction) {
	setUpWishlist(wishlist);
	if (testFunction)
		testFunction(wishlist);
	tearDownWishlist(wishlist);
}


void testRemoveAllActivitiesFromWishlist(ActivityWishlist& wishlist) {
	assert(wishlist.getWishlistSize() == 4);
	wishlist.removeAllActivitiesFromWishlist();

	assert(wishlist.getWishlistSize() == 0);

}
void testAddActivityToWishlist(ActivityWishlist& wishlist) {
	assert(wishlist.getWishlistSize() == 4);
	wishlist.addActivityToWishlist("fishing");

	assert(wishlist.getWishlistSize() == 5);

	assert(wishlist.getActivities().at(4).getActivityName() == "fishing");

	try { wishlist.addActivityToWishlist("some name"); assert(false); }
	catch (const RepositoryException& exception) { assert(exception.getExceptionMessage() == "Activity doesn't exist!\n"); }
}

void testGenerateRandomWishlist(ActivityWishlist& wishlist) {
	wishlist.generateRandomWishlist(2);
	assert(wishlist.getWishlistSize() == 2);
	wishlist.generateRandomWishlist(5);
	assert(wishlist.getWishlistSize() == 5);
}

#include <fstream>

void testExportWishlistToCsvFile(ActivityWishlist& wishlist) {
	assert(wishlist.getWishlistSize() == 4);
	wishlist.exportWishlistToCsvFile("test_file_CSV.txt");
	
	std::ifstream testFileInputStream;
	testFileInputStream.open("test_file_CSV.txt");

	std::vector<Activity> activitiesFromFile;
	int activitiesNumber = 4;
	while (activitiesNumber>0)
	{
		std::string name, description, type, durationString;
		std::getline(testFileInputStream, name,',');
		std::getline(testFileInputStream, description, ',');
		std::getline(testFileInputStream, type, ',');
		std::getline(testFileInputStream, durationString, '\n');
		activitiesFromFile.emplace_back(name, description, type, std::stoi(durationString));
		activitiesNumber--;
	}
	testFileInputStream.close();
	assert(activitiesFromFile.size() == 4);

	const auto& activitiesFromWishlist = wishlist.getActivities();
	for (size_t i = 0; i < 4; i++)
		checkIfActivitiesAreIdentical(activitiesFromFile.at(i), activitiesFromWishlist.at(i));
}

void testGetWishlist(ActivityWishlist& wishlist) {
	assert(wishlist.getWishlistSize() == 4);
	assert(wishlist.getActivities().at(0).getActivityName() == "fishing");
	assert(wishlist.getActivities().at(1).getActivityName() == "dance class");
	assert(wishlist.getActivities().at(2).getActivityName() == "guitar class");
	assert(wishlist.getActivities().at(3).getActivityName() == "kayaking");
	wishlist.removeAllActivitiesFromWishlist();
	assert(wishlist.getActivities().size() == 0);
	assert(wishlist.getWishlistSize() == 0);
}

void testWishlist() {
	ActivityRepositoryInMemory repository;
	ActivityWishlist wishlist(repository);
	setUpRepository(repository);
	setUpTestWishlistAndTearDown(wishlist, testRemoveAllActivitiesFromWishlist);
	setUpTestWishlistAndTearDown(wishlist, testAddActivityToWishlist);
	setUpTestWishlistAndTearDown(wishlist, testGenerateRandomWishlist);
	setUpTestWishlistAndTearDown(wishlist, testExportWishlistToCsvFile);
	setUpTestWishlistAndTearDown(wishlist, testGetWishlist);
}

void testServiceWithUndo(ActivityManagerServiceWithWishlistAndUndo& service)
{
	setUpService(service);

	assert(service.getNumberOfActivitiesService() == 4);

	service.removeActivityByNameService("fishing");
	service.removeActivityByNameService("guitar class");

	assert(service.getNumberOfActivitiesService() == 2);

	service.updateActivityService("kayaking", "k", "k", 10);
	service.updateActivityService("dance class", "d", "d", 11);

	checkIfActivitiesAreIdentical(service.findActivityByNameService("dance class"), { "dance class", "d", "d", 11 });
	checkIfActivitiesAreIdentical(service.findActivityByNameService("kayaking"), { "kayaking", "k", "k", 10 });

	service.undoService();

	checkIfActivitiesAreIdentical(service.findActivityByNameService("dance class"), { "dance class", "course", "indoor", 200 });

	service.undoService();

	checkIfActivitiesAreIdentical(service.findActivityByNameService("kayaking"), { "kayaking", "trip", "outdoor", 300 });

	assert(service.getNumberOfActivitiesService() == 2);

	service.undoService();
	assert(service.getNumberOfActivitiesService() == 3);

	service.undoService();
	assert(service.getNumberOfActivitiesService() == 4);

	service.undoService();
	assert(service.getNumberOfActivitiesService() == 3);

	service.undoService();
	assert(service.getNumberOfActivitiesService() == 2);

	service.undoService();
	assert(service.getNumberOfActivitiesService() == 1);

	service.undoService();
	assert(service.getNumberOfActivitiesService() == 0);

	try { service.undoService(); assert(false); }
	catch (const ControllerException& exception) { assert(exception.getExceptionMessage() == "Can't do undo!\n"); }

	tearDownService(service);
}

void testServiceWithWishListUndoAndFileRepo(const std::string& fileName)
{
	ActivityRepositoryInFile repository(fileName);
	ActivityWishlist wishlist(repository);
	ActivityValidator validator;
	ActivityManagerServiceWithWishlistAndUndo service(repository, validator, wishlist);


	testServiceWithUndo(service);
	testService(service);
	testServiceWishlistMethods(service);
}

void testAll()
{
	testDomain();
	
	createAndTestRepository();
	createAndTestRepositoryWithFile("test_file_1.txt");
	
	createAndTestService();
	createAndTestServiceWithWishlist();
	createAndTestServiceWithFile("test_file_2.txt");

	testServiceWithWishListUndoAndFileRepo("test_file_2.txt");
	
	testUtils();
	testWishlist();
	std::cout << "All tests passed! :D\n";
}
