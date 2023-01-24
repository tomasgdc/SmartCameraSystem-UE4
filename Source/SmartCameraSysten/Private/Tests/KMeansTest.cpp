// Copyright Epic Games, Inc. All Rights Reserved.
#include "CoreTypes.h"
#include "Misc/AutomationTest.h"

//File reading
#include <fstream> 
#include <iostream> 
#include <sstream>

//Plugin
#include "KMeans.h"

//UE4
#include "Misc/FileHelper.h"

#if WITH_DEV_AUTOMATION_TESTS

/*

namespace
{

	std::vector<FPoint> ReadCSV()
	{
		std::vector<FPoint> points;
		std::string line;

		FString loadedFile;
		FString fileDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir() + "SmartCameraSystem/Source/SmartCameraSysten/Private/Tests/kMeansTestData.csv");
		FFileHelper::LoadFileToString(loadedFile, *fileDir);

		std::ifstream file(*fileDir);

		while (getline(file, line)) 
		{
			std::stringstream lineStream(line);
			std::string bit;

			FVector location;
			getline(lineStream, bit, ',');
			location.X = stof(bit);

			getline(lineStream, bit, '\n');
			location.Y = stof(bit);

			points.push_back(FPoint(location));
		}
		return points;
	}

	void WriteCSV(const std::vector<FPoint>& points)
	{
		std::ofstream myfile;

		FString fileDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir() + "SmartCameraSystem/Source/SmartCameraSysten/Private/Tests/kMeansTestOutData.csv");
		myfile.open(*fileDir);
		myfile << "x,y,c" << std::endl;

		for (std::vector<FPoint>::const_iterator it = points.cbegin(); it != points.cend(); ++it)
		{
			myfile << it->location.X << "," << it->location.Y << "," << it->cluster << std::endl;
		}

		myfile.close();
	}
}

*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FKMeansTest, "SmartCameraSystem.KMeans", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)
bool FKMeansTest::RunTest(const FString& Parameters)
{
	int32 numbClusters = 5;
	int32 maxIterations = 2;

	/*
	//Read from file
	std::vector<FPoint> dataPoints = ReadCSV();
	if (dataPoints.size() == 0)
	{
		return false;
	}

	//Init
	KMeans kMeans(numbClusters, maxIterations);

	//Fit model
	kMeans.Fit(dataPoints);

	//Assign cluster to each example
	std::vector<FPoint> predData = kMeans.Predict(dataPoints);

	//Write to file
	//WriteCSV(predData);
	*/

	return true;
 }

#endif //WITH_DEV_AUTOMATION_TESTS