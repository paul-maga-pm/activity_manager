#pragma once
class AplicationCoordinator
{
public:
	AplicationCoordinator() = default;
	~AplicationCoordinator() = default;
	AplicationCoordinator(const AplicationCoordinator& other) = delete;
	AplicationCoordinator& operator=(const AplicationCoordinator& other) = delete;
	AplicationCoordinator(AplicationCoordinator&& other) = delete;
	AplicationCoordinator& operator=(AplicationCoordinator&& other) = delete;
	void setUpAndRunApplication();
};

