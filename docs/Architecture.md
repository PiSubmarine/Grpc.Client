# Grpc.Client

`PiSubmarine.Grpc.Client` owns the shared client-side gRPC transport channel
used by composition roots.

## Responsibility

This module owns:

- mutual TLS client configuration
- creation of a shared `grpc::Channel`
- RPC timeout configuration reused by service-specific clients

It does not own:

- domain RPC methods
- protobuf request mapping
- service-specific result translation
