#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "PiSubmarine/Logging/Api/IFactory.h"

namespace grpc
{
    class Channel;
}

namespace PiSubmarine::Grpc::Client
{
    struct TlsConfig
    {
        std::string Target;
        std::string CertificateAuthority;
        std::string ClientCertificateChain;
        std::string ClientPrivateKey;
        std::string ServerAuthorityOverride;
        std::chrono::milliseconds RpcTimeout{5000};
    };

    class Channel
    {
    public:
        Channel(Logging::Api::IFactory& loggerFactory, TlsConfig tlsConfig);

        [[nodiscard]] const std::shared_ptr<::grpc::Channel>& Get() const noexcept;
        [[nodiscard]] std::chrono::milliseconds GetRpcTimeout() const noexcept;

    private:
        TlsConfig m_TlsConfig;
        std::shared_ptr<spdlog::logger> m_Logger;
        std::shared_ptr<::grpc::Channel> m_Channel;
    };
}
