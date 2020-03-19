/*
 * Generated by asn1c-0.9.29 n1 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-15-04.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#ifndef	_X2N_DataTrafficResourceIndication_H_
#define	_X2N_DataTrafficResourceIndication_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "X2N_SharedResourceType.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct X2N_ReservedSubframePattern;
struct X2N_ProtocolExtensionContainer;

/* X2N_DataTrafficResourceIndication */
typedef struct X2N_DataTrafficResourceIndication {
	long	 activationSFN;
	X2N_SharedResourceType_t	 sharedResourceType;
	struct X2N_ReservedSubframePattern	*reservedSubframePattern;	/* OPTIONAL */
	struct X2N_ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} X2N_DataTrafficResourceIndication_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_X2N_DataTrafficResourceIndication;

#ifdef __cplusplus
}
#endif

#endif	/* _X2N_DataTrafficResourceIndication_H_ */
#include <asn_internal.h>
