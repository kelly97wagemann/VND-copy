//
// Generated file, do not edit! Created by nedtool 5.4 from veins/modules/messages/AirFrame11p.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __AIRFRAME11P_M_H
#define __AIRFRAME11P_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "veins/base/messages/AirFrame_m.h"
using Veins::AirFrame;
// }}

/**
 * Class generated from <tt>veins/modules/messages/AirFrame11p.msg:25</tt> by nedtool.
 * <pre>
 * //
 * // Extension of base AirFrame message to have the underSensitivity field
 * //
 * message AirFrame11p extends AirFrame
 * {
 *     bool underSensitivity = false;
 *     bool wasTransmitting = false;
 * }
 * </pre>
 */
class AirFrame11p : public ::AirFrame
{
  protected:
    bool underSensitivity;
    bool wasTransmitting;

  private:
    void copy(const AirFrame11p& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AirFrame11p&);

  public:
    AirFrame11p(const char *name=nullptr, short kind=0);
    AirFrame11p(const AirFrame11p& other);
    virtual ~AirFrame11p();
    AirFrame11p& operator=(const AirFrame11p& other);
    virtual AirFrame11p *dup() const override {return new AirFrame11p(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual bool getUnderSensitivity() const;
    virtual void setUnderSensitivity(bool underSensitivity);
    virtual bool getWasTransmitting() const;
    virtual void setWasTransmitting(bool wasTransmitting);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AirFrame11p& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AirFrame11p& obj) {obj.parsimUnpack(b);}


#endif // ifndef __AIRFRAME11P_M_H

