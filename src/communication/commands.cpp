#include "commands.h"
#include "shared/qt/communication/functions.h"
#include "shared/qt/communication/commands_pool.h"

namespace communication {
namespace command {

#define REGISTRY_COMMAND_SINGLPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = CommandsPool::Registry{UUID, #COMMAND, false};

#define REGISTRY_COMMAND_MULTIPROC(COMMAND, UUID) \
    const QUuidEx COMMAND = CommandsPool::Registry{UUID, #COMMAND, true};

REGISTRY_COMMAND_SINGLPROC(TestCommand1,     "5b3f0fb8-61f6-43e5-9042-f15e4d9234a6")
REGISTRY_COMMAND_SINGLPROC(TestCommand2,     "2c8bd14d-cd98-4e59-86f5-739877d8718c")
REGISTRY_COMMAND_SINGLPROC(TestCommand3,     "ec043e01-47f4-4ab8-a5f6-5ab9b2022842")


#undef REGISTRY_COMMAND_SINGLPROC
#undef REGISTRY_COMMAND_MULTIPROC
} // namespace command

namespace data {

} // namespace data
} // namespace communication
