/*! \mainpage Process the raw data from the Multi-blade detector.
 *
 * This class processes data-points from one cassette of the multi-blade detector.
 *
 * The data-points are first formed into clusters within a certain preset time-window. The clusters are then check for
 * adjacency and if passed the position of the cluster will be determined. The position can be either determined as the
 * wire/strip with maximum signal of the cluster or be calculated using a weighted average with the signals as the
 * weights.
 *
 * This class is a reimplementation of clustering algorithm written in MatLab by
 * Francesco Piscitelli <Francesco.Piscitelli@esss.se>.
 *
 * - \subpage clustering
 *
 * - \subpage checks
 *
 * - \subpage position
 *
 * - \author Carsten Søgaard \n Niels Bohr Institute, University of Copenhagen \n <soegaard@nbi.dk>
 *
 */

// The remaining documentation is found in the docu.dox file

#ifndef MULTIBLADE_EVENTBUILDER_H
#define MULTIBLADE_EVENTBUILDER_H

#include <array>
#include <vector>
#include <cstdint>
#include <iostream>

/*! Struct containing channel number and ADC-value for a singe strip or wire. */
struct datapoint
{
    uint8_t  channel; /*!< Number of either wire or strip channel */
    uint64_t ADC; /*!< Value of the signal form the corresponding channel */
};

/*! Overloaded < operator. Required when we need to sort the data points for later neighbour check
 *
 * @param a datapoint 1
 * @param b datapoint 2
 * @return bool
 */
bool operator< (const datapoint& a, const datapoint& b);

class multiBladeEventBuilder {

public:

    /*! Constructor */
    multiBladeEventBuilder();

    /*! Destructor */
    ~multiBladeEventBuilder();

    /*! Data-points from one multi-blade detector cassette consists of a channel number, a signal value from the
     * channel and a clock-cycle number. Each data point is added through this function. The first sanity check on the
     * data is done here (See \subpage checks). The algorithm building the clusters/events is described in
     * \subpage clustering.
     * Once a cluster is completed the position is calculated automatically and the function returns true. Accepting the
     * true boolean should therefore trigger the readout of the calculated position.
     *
     * @param channel Channel number
     * @param ADC Signal value
     * @param clock Clock-cycle number
     * @return True when the cluster/event is complete, false otherwise
     */
    bool addDataPoint(const uint8_t& channel, const uint64_t& ADC, const uint32_t& clock);

    /*! Call this function when last point of the run has been received and the stored cluster will be processed.
     */
    void lastPoint();

    /*! Returns the calculated wire position. The value is only updated when a cluster/event is complete.
     * One should therefore only call this function when addDataPoint() returns true.
     *
     * @return Calculated wire position
     */
    double getWirePosition() {return m_wire_pos;}

    /*! Returns the calculated strip position. The value is only updated when a cluster/event is complete.
     * One should therefore only call this function when addDataPoint() returns true.
     *
     * @return Calculated strip position
     */
    double getStripPosition() {return m_strip_pos;}

    /*! Returns the timestamp of the cluster currently being processed.
     *
     *
     * @return The time-stamp in seconds.
     */
    double getTimeStamp() { return m_time_stamp; }


    /*! Returns the cluster position [wire, strip, time-stamp]
     *
     * @return  Cluster position
     */
    std::vector<double> getPosition();

    // Functions for configuration

    /*! Set the duration of one clock-cycle in seconds. This value is used to determine the time-stamp of the
     * event/cluster into seconds.
     *
     * @param clock_d Clock-cycle duration.
     */
    void setClockDuration(double clock_d) {m_clock_d = clock_d;}

    /*! Set the time-window for accepting data-points as belonging to the same cluster. The time-stamps of the
     * data-points are in number of clock-cycles (since last reset). Therefore the time-window must be specified as
     * the maximum number of clock-cycles after the first data-point in which a point will be added to the cluster.
     *
     * @param time_window Cluster time-window.
     */
    void setTimeWindow(uint32_t time_window) {m_time_window = time_window;}

    /*! Toggle whether or not to use weighted average as position for wire and strip.
     *
     * @param set Set to true to toggle (default)
     */
    void setUseWeightedAverage(bool set = true) {m_use_weighted_average  = set;}

    /*! Set the number of wire channels in the cassette. Channels with higher numbers than this will be assumed to
     * be strips channels. Channel numbers higher than the sum of wire and strip channels will be considered invalid.
     * @param nchannels
     */
    void setNumberOfWireChannels(uint8_t nchannels) { m_nwire_channels = nchannels;}

    /*! Set the number of strip channels in the cassette. Channel numbers higher than the sum of wire and strip
     * channels will be considered invalid.
     * @param nchannels
     */
    void setNumberOfStripChannels(uint8_t nchannels) { m_nstrip_channels = nchannels;}

    /*! Set the output to be verbose. Use only with limited number of data-points as the output will be quite massive.
     * For debugging purposes only.
     * @param set
     */
    void setVerbose(bool set = true) {m_verbose = set;}

    // Functions for monitoring and/or debugging

    /*! Return the number of data-points received */
    uint64_t getNumberOfDatapointsReceived() { return m_datapoints_received; }
    /*! Return the number of events */
    uint64_t getNumberOfEvents() { return m_nevents; }
    /*! Return the number of events rejected due to not being adjacent */
    uint64_t getNumberOfAdjacencyRejected() { return m_rejected_adjacency; }
    /*! Return the number of events rejected due to not having a valid position */
    uint64_t getNumberOfPositionRejected() { return m_rejected_position; }

    /*! Returns the 2D-counter for wire clusters. */
    std::array<uint64_t, 6> get2DWireClusterCounter() { return m_2D_wires; }
    /*! Returns the 2D-counter for strip clusters */
    std::array<uint64_t, 6> get2DStripClusterCounter() { return m_2D_strips; }
    /*! Returns the 1D-counter for wire clusters */
    std::array<uint64_t, 6> get1DWireClusterCounter() { return m_1D_wires; }
    /*! Returns the 1D-counter for strip clusters */
    std::array<uint64_t, 6> get1DStripClusterCounter() { return m_1D_strips; }

    /*! Return the size of the wire signal cluster
     * Probably only useful for real-time debugging */
    unsigned long getWireClusterSize() { return m_wire_cluster.size(); }
    /*! Return the size of the strip signal cluster
     * Probably only useful for real-time debugging */
    unsigned long getStripClusterSize() { return m_strip_cluster.size(); }

    /*! Return the clock-cycle number for the current cluster
     * Useful only for debugging
     *
     * @return Clock-cycle number for the current cluster
     */
    u_int32_t getClusterClock() { return m_cluster_clock; }

    /*! Reset debugging and monitoring counters */
    void resetCounters();

private:

    // Configuration variables
    /*! Duration of one clock-cycle in seconds*/
    double m_clock_d;
    /*! Time window for a single cluster. In number of clock-cycles */
    uint32_t m_time_window;
    /*! Number of wire channels */
    uint8_t m_nwire_channels;
    /*! Number of strip channels */
    uint8_t m_nstrip_channels;
    /*! Whether to use wieghted average or not when calculation position */
    bool m_use_weighted_average;

    // Runtime variables
    /*! Container for one wire cluster */
    std::vector<datapoint> m_wire_cluster;
    /*! Container for one strip cluster */
    std::vector<datapoint> m_strip_cluster;
    /*! Container for the calculated wire position */
    double m_wire_pos;
    /*! Container for the calculated strip position */
    double m_strip_pos;
    /*! Timestamp of the processed cluster */
    double m_time_stamp;
    /*! Timestamp of current cluster. Also the beginning of time-window. */
    uint32_t m_cluster_clock;
    /*! True only at the beginning of each cluster */
    bool m_first_signal;
    /*! Set verbose output */
    bool m_verbose;

    // Monitoring/debugging counters
    uint64_t m_datapoints_received; /*! Number of received data-points */
    uint64_t m_nevents; /*! Number of valid events */
    uint64_t m_rejected_adjacency; /*! Number of events rejected due to adjacency */
    uint64_t m_rejected_position; /*! Number of events rejected due to position */
    std::array<uint64_t, 6> m_2D_wires;  /*!< 2D clusters. Number of wires fired. >5 wires -> element 6 */
    std::array<uint64_t, 6> m_2D_strips; /*!< 2D clusters. Number of strips fired. >5 strips -> element 6 */
    std::array<uint64_t, 6> m_1D_wires;  /*!< 1D clusters. Number of wires fired. >5 wires -> element 6 */
    std::array<uint64_t, 6> m_1D_strips; /*!< 1D clusters. Number of strips fired. >5 strips -> element 6 */

    // Private member functions

    /*! Adds a point to either wire-cluster or strip-cluster
     *
     * @param channel Channel number
     * @param ADC Signal value
     */
    void addPointToCluster(uint8_t channel, uint64_t ADC);

    /*! This function is called once a cluster is formed */
    bool processClusters();

    bool pointsAdjacent();

    /*! This function checks that signals originate from either adjacent wires or strips.
     * Currently non-adjacent points are removed.
     * Future implementations could include a mask of dead-channels as well as formation of additional clusters
     * from the discarded points.
     *
     * @param cluster Cluster of wire and strip signals with the time-window.
     * @return True if points are adjacent, false if not
     */
    bool checkAdjacency(std::vector<datapoint> cluster);

     /*! Function to calculate either wire or strip position.
     * Will use weigthed average or max ADC as selected.
     * Returns -1 if position cannot be determined
     * @param cluster
     * @return Position of the cluster (wire or strip)
     */
    double calculatePosition(std::vector<datapoint> cluster);

    /*! Increments monitoring counters.  */
    void incrementCounters(std::vector<datapoint> m_wire_cluster, std::vector<datapoint> m_strip_cluster);
};


#endif //MULTIBLADE_EVENTBUILDER_H
