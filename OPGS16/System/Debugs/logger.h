#ifndef OPGS16_SYSTEM_DEBUGS_LOGGER_H
#define OPGS16_SYSTEM_DEBUGS_LOGGER_H

/**
 * @namespace debug
 * @brief Namespace for debug components.
 */
namespace debug {

/**
 * @class Logger
 * @brief Logger static sub-program.
 */
class Logger final {
public:
	static Logger& GetInstance() {
		static Logger instance{};
		return instance;
	}

private:

};

}

#endif /** OPGS16_SYSTEM_DEBUGS_LOGGER_H */
