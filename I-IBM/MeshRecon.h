#pragma once
class MeshRecon
{
public:
	MeshRecon();
	~MeshRecon();

	static void PoissonRecon(const char* inputplypointfile, const char* outputplymeshfile);

};

