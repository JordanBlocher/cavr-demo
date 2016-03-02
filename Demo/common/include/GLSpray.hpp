#include <GLRibbon.hpp>
#include <GLPrimitive.hpp>
class GLSpray
{
public:
	GLSpray();
	void AddSpray();
	void Clear();

private:
	std::vector<GLPrimitive> Primitives;
	// we want only one ribbon class.
	GLRibbon ribbons;	
};