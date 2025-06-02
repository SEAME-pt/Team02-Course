# ADR 0003: Zenoh Router with InfluxDB Backend

## Status
Accepted

## Context
To enable efficient communication between the car and the cloud, we need a robust data routing and storage solution. Additionally, we aim to visualize measurements using Grafana.

## Decision
We will deploy a Zenoh router running in the car, configured with an InfluxDB backend. This setup will allow:
- Real-time communication between the car and the cloud.
- Storage of measurement data in InfluxDB for later analysis.
- Integration with Grafana to create visual dashboards for measurement data.

## Consequences
- The Zenoh router will handle data routing efficiently, enabling seamless cloud communication.
- InfluxDB will provide a reliable backend for storing time-series data.
- Future integration with Grafana will allow us to generate insightful charts and dashboards.
- Additional resources will be required to maintain the Zenoh router and InfluxDB setup.

## Alternatives Considered
- Using a different message broker or database backend, but Zenoh and InfluxDB were chosen for their performance and compatibility with our requirements.
