/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 NITK Surathkal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Avinash <avinash.kumar369@gmail.com>
 *         Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

#include "tcp-rate-halving-recovery.h"
#include "ns3/tcp-socket-base.h"
#include "ns3/tcp-congestion-ops.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("TcpRateHalvingRecovery");
NS_OBJECT_ENSURE_REGISTERED (TcpRateHalvingRecovery);

TypeId
TcpRateHalvingRecovery::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TcpRateHalvingRecovery")
    .SetParent<TcpClassicRecovery> ()
    .AddConstructor<TcpRateHalvingRecovery> ()
    .SetGroupName ("Internet")
  ;
  return tid;
}

TcpRateHalvingRecovery::TcpRateHalvingRecovery (void)
  : TcpClassicRecovery ()
{
  NS_LOG_FUNCTION (this);
}

TcpRateHalvingRecovery::TcpRateHalvingRecovery (const TcpRateHalvingRecovery& recovery)
  : TcpClassicRecovery (recovery),
  m_isAlternateAck (recovery.m_isAlternateAck)
{
  NS_LOG_FUNCTION (this);
}

TcpRateHalvingRecovery::~TcpRateHalvingRecovery (void)
{
  NS_LOG_FUNCTION (this);
}

void
TcpRateHalvingRecovery::EnterRecovery (Ptr<TcpSocketState> tcb, uint32_t dupAckCount,
                                       uint32_t unAckDataCount, uint32_t lastSackedBytes)
{
  NS_LOG_FUNCTION (this << tcb << dupAckCount << unAckDataCount << lastSackedBytes);
  NS_UNUSED (dupAckCount);
  NS_UNUSED (unAckDataCount);
  NS_UNUSED (lastSackedBytes);

  m_isAlternateAck = false;

  DoRecovery (tcb, 0, lastSackedBytes);
}

void
TcpRateHalvingRecovery::DoRecovery (Ptr<TcpSocketState> tcb, uint32_t lastAckedBytes,
                                    uint32_t lastSackedBytes)
{
  NS_LOG_FUNCTION (this << tcb << lastAckedBytes << lastSackedBytes);

  int sendCount = 0;
  if (tcb->m_bytesInFlight > tcb->m_ssThresh)
    {
      if (m_isAlternateAck)
        {
          m_isAlternateAck = false;
          sendCount = 1 * tcb->m_segmentSize;
        }
      else 
        {
          m_isAlternateAck = true;
          sendCount = 0;
        }
    }

  else
    {
      tcb->m_cWnd = tcb->m_ssThresh;
    }
  tcb->m_cWnd = tcb->m_bytesInFlight + sendCount;
  tcb->m_cWndInfl = tcb->m_cWnd;
}

void 
TcpRateHalvingRecovery::ExitRecovery (Ptr<TcpSocketState> tcb)
{
  NS_LOG_FUNCTION (this << tcb);
  tcb->m_cWnd = tcb->m_ssThresh.Get ();
  tcb->m_cWndInfl = tcb->m_cWnd;
}


Ptr<TcpRecoveryOps>
TcpRateHalvingRecovery::Fork (void)
{
  return CopyObject<TcpRateHalvingRecovery> (this);
}

std::string
TcpRateHalvingRecovery::GetName () const
{
  return "RateHalvingRecovery";
}

} // namespace ns3
