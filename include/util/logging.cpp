

#include "util/logging.hpp"


namespace ay::logging
{


void init_logging([[maybe_unused]] bool debug, [[maybe_unused]] bool stand)
{
#ifdef DEBUG_LOGGING
    LOGGER = standard_logger(debug, stand);
#endif
}


}  // namespace ay::logging
