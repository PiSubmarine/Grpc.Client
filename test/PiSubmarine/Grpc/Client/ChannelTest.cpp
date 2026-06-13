#include <gtest/gtest.h>
#include <memory>
#include <spdlog/logger.h>
#include <spdlog/sinks/null_sink.h>

#include "PiSubmarine/Grpc/Client/Channel.h"

namespace PiSubmarine::Grpc::Client
{
    namespace
    {
        class NullLoggerFactory final : public Logging::Api::IFactory
        {
        public:
            [[nodiscard]] std::shared_ptr<spdlog::logger> CreateLogger(std::string_view) override
            {
                return {};
            }
        };

        class LoggerFactoryStub final : public Logging::Api::IFactory
        {
        public:
            [[nodiscard]] std::shared_ptr<spdlog::logger> CreateLogger(std::string_view name) override
            {
                return std::make_shared<spdlog::logger>(
                    std::string(name),
                    std::make_shared<spdlog::sinks::null_sink_mt>());
            }
        };

        [[nodiscard]] TlsConfig MakeTlsConfig()
        {
            return TlsConfig{
                .Target = "127.0.0.1:50051",
                .CertificateAuthority = "ca",
                .ClientCertificateChain = "cert",
                .ClientPrivateKey = "key"};
        }
    }

    TEST(ChannelTest, RequiresLoggerFactoryToReturnLogger)
    {
        NullLoggerFactory loggerFactory;

        EXPECT_THROW((Channel(loggerFactory, MakeTlsConfig())), std::invalid_argument);
    }

    TEST(ChannelTest, RequiresTlsConfiguration)
    {
        LoggerFactoryStub loggerFactory;

        EXPECT_THROW((Channel(loggerFactory, TlsConfig{})), std::invalid_argument);
    }

    TEST(ChannelTest, ConstructsWithValidConfiguration)
    {
        LoggerFactoryStub loggerFactory;

        EXPECT_NO_THROW((Channel(loggerFactory, MakeTlsConfig())));
    }
}
