#include "AmpEventDelay.h"
#include <random>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

std::random_device RandomDevice;
std::default_random_engine Generator(RandomDevice());
std::uniform_real_distribution<double> DistributionPi(0, 3.141592653);

auto generateCircleAmplitudes() {
  const double Amplitude{2000};
  const double Center{3000};
  auto Angle = DistributionPi(Generator);
  return std::make_pair(Center + Amplitude * std::cos(Angle),
                        Center + Amplitude * std::sin(Angle));
}

void AmpEventDelay::genSamplesAndQueueSend(const TimeStamp& Time) {
 
  auto SampleRunAnode = data.AnodeGen.generate(2000.0, Time);
  auto Amplitudes = generateCircleAmplitudes();
  auto SampleRunX = data.XPosGen.generate(Amplitudes.first, Time);
  auto SampleRunY = data.YPosGen.generate(Amplitudes.second, Time);
  data.PoissonData.TimerData.UdpCon->addSamplingRun(
      SampleRunAnode.first, SampleRunAnode.second, Time);
  data.PoissonData.TimerData.UdpCon->addSamplingRun(SampleRunX.first,
                                                    SampleRunX.second, Time);
  data.PoissonData.TimerData.UdpCon->addSamplingRun(SampleRunY.first,
                                                    SampleRunY.second, Time);
}