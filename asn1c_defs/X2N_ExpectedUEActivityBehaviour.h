/*
 * Generated by asn1c-0.9.29 n1 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-15-04.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#ifndef	_X2N_ExpectedUEActivityBehaviour_H_
#define	_X2N_ExpectedUEActivityBehaviour_H_


#include <asn_application.h>

/* Including external dependencies */
#include "X2N_ExpectedActivityPeriod.h"
#include "X2N_ExpectedIdlePeriod.h"
#include "X2N_SourceOfUEActivityBehaviourInformation.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct X2N_ProtocolExtensionContainer;

/* X2N_ExpectedUEActivityBehaviour */
typedef struct X2N_ExpectedUEActivityBehaviour {
	X2N_ExpectedActivityPeriod_t	*expectedActivityPeriod;	/* OPTIONAL */
	X2N_ExpectedIdlePeriod_t	*expectedIdlePeriod;	/* OPTIONAL */
	X2N_SourceOfUEActivityBehaviourInformation_t	*sourceofUEActivityBehaviourInformation;	/* OPTIONAL */
	struct X2N_ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} X2N_ExpectedUEActivityBehaviour_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_X2N_ExpectedUEActivityBehaviour;
extern asn_SEQUENCE_specifics_t asn_SPC_X2N_ExpectedUEActivityBehaviour_specs_1;
extern asn_TYPE_member_t asn_MBR_X2N_ExpectedUEActivityBehaviour_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _X2N_ExpectedUEActivityBehaviour_H_ */
#include <asn_internal.h>