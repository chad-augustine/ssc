#ifndef __LIB_BATTERY_POWERFLOW_TEST_H__
#define __LIB_BATTERY_POWERFLOW_TEST_H__

#include <gtest/gtest.h>
#include <lib_battery_powerflow.h>
#include <lib_power_electronics.h>


class BatteryPowerFlowTest : public ::testing::Test
{
protected:
	BatteryPowerFlow * m_batteryPowerFlow;
	BatteryPower * m_batteryPower;
	double error;

public:

	void SetUp()
	{
		error = 0.01;
		double dtHour = 1.0;
		m_batteryPowerFlow = new BatteryPowerFlow(dtHour);
		m_batteryPower = m_batteryPowerFlow->getBatteryPower();
		m_batteryPower->canDischarge = false;
		m_batteryPower->canPVCharge = false;
		m_batteryPower->canGridCharge = false;
		m_batteryPower->singlePointEfficiencyACToDC = 0.96;
		m_batteryPower->singlePointEfficiencyDCToAC = 0.96;
		m_batteryPower->powerBatteryChargeMax = 100;
		m_batteryPower->powerBatteryDischargeMax = 50;
		m_batteryPower->connectionMode = ChargeController::AC_CONNECTED;
	}
	void TearDown()
	{
		if (m_batteryPowerFlow)
			delete m_batteryPowerFlow;
	}

};



#endif