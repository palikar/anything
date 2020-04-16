

#include "util/logging.hpp"


namespace ay
{


void init_logging([[maybe_unused]] bool debug, [[maybe_unused]] bool stand)
{
#ifdef DEUBG_LOGGING
    LOGGER = standard_logger(debug, stand);
#endif
}


}  // namespace ay
