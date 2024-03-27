#ifndef V3_WORLD_H
#define V3_WORLD_H
#include "Configuration.h"
#include "DatingData.h"

namespace EU5
{
class World;
}

namespace V3
{
class World
{
  public:
	World(const Configuration& configuration, const EU5::World& sourceWorld);

		[[nodiscard]] const auto& getDatingData() const { return datingData; }

  private:
	std::string V3Path;
	Configuration::ConfigBlock configBlock;
	DatingData datingData;

};

} // namespace V3

#endif // V3_WORLD_H
