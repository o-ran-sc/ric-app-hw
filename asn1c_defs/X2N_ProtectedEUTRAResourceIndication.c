/*
 * Generated by asn1c-0.9.29 n1 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-15-04.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#include "X2N_ProtectedEUTRAResourceIndication.h"

#include "X2N_ProtocolExtensionContainer.h"
static int
memb_X2N_activationSFN_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1023)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_X2N_mBSFNControlRegionLength_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 3)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_X2N_pDCCHRegionLength_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 1 && value <= 3)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_X2N_activationSFN_constr_2 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 10,  10,  0,  1023 }	/* (0..1023) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_X2N_mBSFNControlRegionLength_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  0,  3 }	/* (0..3) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_X2N_pDCCHRegionLength_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 2,  2,  1,  3 }	/* (1..3) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_X2N_ProtectedEUTRAResourceIndication_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_ProtectedEUTRAResourceIndication, activationSFN),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ 0, &asn_PER_memb_X2N_activationSFN_constr_2,  memb_X2N_activationSFN_constraint_1 },
		0, 0, /* No default value */
		"activationSFN"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_ProtectedEUTRAResourceIndication, protectedResourceList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ProtectedResourceList,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"protectedResourceList"
		},
	{ ATF_POINTER, 3, offsetof(struct X2N_ProtectedEUTRAResourceIndication, mBSFNControlRegionLength),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ 0, &asn_PER_memb_X2N_mBSFNControlRegionLength_constr_4,  memb_X2N_mBSFNControlRegionLength_constraint_1 },
		0, 0, /* No default value */
		"mBSFNControlRegionLength"
		},
	{ ATF_POINTER, 2, offsetof(struct X2N_ProtectedEUTRAResourceIndication, pDCCHRegionLength),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,
		{ 0, &asn_PER_memb_X2N_pDCCHRegionLength_constr_5,  memb_X2N_pDCCHRegionLength_constraint_1 },
		0, 0, /* No default value */
		"pDCCHRegionLength"
		},
	{ ATF_POINTER, 1, offsetof(struct X2N_ProtectedEUTRAResourceIndication, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ProtocolExtensionContainer_8231P180,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_X2N_ProtectedEUTRAResourceIndication_oms_1[] = { 2, 3, 4 };
static const ber_tlv_tag_t asn_DEF_X2N_ProtectedEUTRAResourceIndication_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_X2N_ProtectedEUTRAResourceIndication_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* activationSFN */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* protectedResourceList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* mBSFNControlRegionLength */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* pDCCHRegionLength */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* iE-Extensions */
};
static asn_SEQUENCE_specifics_t asn_SPC_X2N_ProtectedEUTRAResourceIndication_specs_1 = {
	sizeof(struct X2N_ProtectedEUTRAResourceIndication),
	offsetof(struct X2N_ProtectedEUTRAResourceIndication, _asn_ctx),
	asn_MAP_X2N_ProtectedEUTRAResourceIndication_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_X2N_ProtectedEUTRAResourceIndication_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	5,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_X2N_ProtectedEUTRAResourceIndication = {
	"ProtectedEUTRAResourceIndication",
	"ProtectedEUTRAResourceIndication",
	&asn_OP_SEQUENCE,
	asn_DEF_X2N_ProtectedEUTRAResourceIndication_tags_1,
	sizeof(asn_DEF_X2N_ProtectedEUTRAResourceIndication_tags_1)
		/sizeof(asn_DEF_X2N_ProtectedEUTRAResourceIndication_tags_1[0]), /* 1 */
	asn_DEF_X2N_ProtectedEUTRAResourceIndication_tags_1,	/* Same as above */
	sizeof(asn_DEF_X2N_ProtectedEUTRAResourceIndication_tags_1)
		/sizeof(asn_DEF_X2N_ProtectedEUTRAResourceIndication_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_X2N_ProtectedEUTRAResourceIndication_1,
	5,	/* Elements count */
	&asn_SPC_X2N_ProtectedEUTRAResourceIndication_specs_1	/* Additional specs */
};

