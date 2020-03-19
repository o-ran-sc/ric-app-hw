/*
 * Generated by asn1c-0.9.29 n1 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-15-04.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#ifndef	_X2N_WidebandCQICodeword1_H_
#define	_X2N_WidebandCQICodeword1_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum X2N_WidebandCQICodeword1_PR {
	X2N_WidebandCQICodeword1_PR_NOTHING,	/* No components present */
	X2N_WidebandCQICodeword1_PR_four_bitCQI,
	X2N_WidebandCQICodeword1_PR_three_bitSpatialDifferentialCQI
	/* Extensions may appear below */
	
} X2N_WidebandCQICodeword1_PR;

/* X2N_WidebandCQICodeword1 */
typedef struct X2N_WidebandCQICodeword1 {
	X2N_WidebandCQICodeword1_PR present;
	union X2N_WidebandCQICodeword1_u {
		long	 four_bitCQI;
		long	 three_bitSpatialDifferentialCQI;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} X2N_WidebandCQICodeword1_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_X2N_WidebandCQICodeword1;
extern asn_CHOICE_specifics_t asn_SPC_X2N_WidebandCQICodeword1_specs_1;
extern asn_TYPE_member_t asn_MBR_X2N_WidebandCQICodeword1_1[2];
extern asn_per_constraints_t asn_PER_type_X2N_WidebandCQICodeword1_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _X2N_WidebandCQICodeword1_H_ */
#include <asn_internal.h>
