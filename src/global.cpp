#include "global.h"

std::string _R2D_ MyPath;

void R2D::_pr_error(std::string l)
{
	std::cerr << "R2D ERROR: " << l << std::endl;
}

void R2D::_pr_warning(std::string l)
{
	std::clog << "R2D WARNING: " << l << std::endl;
}

