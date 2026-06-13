#include "PiSubmarine/Grpc/Client/Channel.h"

#include <stdexcept>

#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>
#include <spdlog/spdlog.h>

namespace PiSubmarine::Grpc::Client
{
    namespace
    {
        [[nodiscard]] std::shared_ptr<spdlog::logger> CreateLogger(Logging::Api::IFactory& loggerFactory)
        {
            auto logger = loggerFactory.CreateLogger("Grpc.Client");
            if (!logger)
            {
                throw std::invalid_argument("Grpc.Client requires a logger factory that returns a logger");
            }

            return logger;
        }
    }

    Channel::Channel(Logging::Api::IFactory& loggerFactory, TlsConfig tlsConfig)
        : m_TlsConfig(std::move(tlsConfig))
        , m_Logger(CreateLogger(loggerFactory))
    {
        if (m_TlsConfig.Target.empty() ||
            m_TlsConfig.CertificateAuthority.empty() ||
            m_TlsConfig.ClientCertificateChain.empty() ||
            m_TlsConfig.ClientPrivateKey.empty())
        {
            SPDLOG_LOGGER_ERROR(m_Logger, "Rejected gRPC client channel construction because TLS configuration is incomplete");
            throw std::invalid_argument("Grpc.Client requires complete mutual TLS configuration");
        }

        ::grpc::SslCredentialsOptions sslOptions;
        sslOptions.pem_root_certs = m_TlsConfig.CertificateAuthority;
        sslOptions.pem_cert_chain = m_TlsConfig.ClientCertificateChain;
        sslOptions.pem_private_key = m_TlsConfig.ClientPrivateKey;

        ::grpc::ChannelArguments channelArguments;
        if (!m_TlsConfig.ServerAuthorityOverride.empty())
        {
            channelArguments.SetSslTargetNameOverride(m_TlsConfig.ServerAuthorityOverride);
        }

        m_Channel = ::grpc::CreateCustomChannel(
            m_TlsConfig.Target,
            ::grpc::SslCredentials(sslOptions),
            channelArguments);

        SPDLOG_LOGGER_INFO(m_Logger, "Initialized shared gRPC client channel for target '{}'", m_TlsConfig.Target);
    }

    const std::shared_ptr<::grpc::Channel>& Channel::Get() const noexcept
    {
        return m_Channel;
    }

    std::chrono::milliseconds Channel::GetRpcTimeout() const noexcept
    {
        return m_TlsConfig.RpcTimeout;
    }
}
