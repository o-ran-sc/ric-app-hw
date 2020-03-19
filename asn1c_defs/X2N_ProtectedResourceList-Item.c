/*
 * Generated by asn1c-0.9.29 n1 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-15-04.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#include "X2N_ProtectedResourceList-Item.h"

#include "X2N_ProtocolExtensionContainer.h"
static int
memb_X2N_intraPRBProtectedResourceFootprint_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 84)) {
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
memb_X2N_protectedFootprintFrequencyPattern_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size >= 6 && size <= 110)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_per_constraints_t asn_PER_memb_X2N_intraPRBProtectedResourceFootprint_constr_3 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  0,  0,  84,  84 }	/* (SIZE(84..84,...)) */,
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_memb_X2N_protectedFootprintFrequencyPattern_constr_4 CC_NOTUSED = {
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	{ APC_CONSTRAINED | APC_EXTENSIBLE,  7,  7,  6,  110 }	/* (SIZE(6..110,...)) */,
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_X2N_ProtectedResourceList_Item_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_ProtectedResourceList_Item, resourceType),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ResourceType,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"resourceType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_ProtectedResourceList_Item, intraPRBProtectedResourceFootprint),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ 0, &asn_PER_memb_X2N_intraPRBProtectedResourceFootprint_constr_3,  memb_X2N_intraPRBProtectedResourceFootprint_constraint_1 },
		0, 0, /* No default value */
		"intraPRBProtectedResourceFootprint"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_ProtectedResourceList_Item, protectedFootprintFrequencyPattern),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BIT_STRING,
		0,
		{ 0, &asn_PER_memb_X2N_protectedFootprintFrequencyPattern_constr_4,  memb_X2N_protectedFootprintFrequencyPattern_constraint_1 },
		0, 0, /* No default value */
		"protectedFootprintFrequencyPattern"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_ProtectedResourceList_Item, protectedFootprintTimePattern),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ProtectedFootprintTimePattern,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"protectedFootprintTimePattern"
		},
	{ ATF_POINTER, 1, offsetof(struct X2N_ProtectedResourceList_Item, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ProtocolExtensionContainer_8231P182,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_X2N_ProtectedResourceList_Item_oms_1[] = { 4 };
static const ber_tlv_tag_t asn_DEF_X2N_ProtectedResourceList_Item_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_X2N_ProtectedResourceList_Item_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* resourceType */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* intraPRBProtectedResourceFootprint */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* protectedFootprintFrequencyPattern */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* protectedFootprintTimePattern */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_X2N_ProtectedResourceList_Item_specs_1 = {
	sizeof(struct X2N_ProtectedResourceList_Item),
	offsetof(struct X2N_ProtectedResourceList_Item, _asn_ctx),
	asn_MAP_X2N_ProtectedResourceList_Item_tag2el_1,
	5,	/* Count of tags in the map */
	asn_MAP_X2N_ProtectedResourceList_Item_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	5,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_X2N_ProtectedResourceList_Item = {
	"ProtectedResourceList-Item",
	"ProtectedResourceList-Item",
	&asn_OP_SEQUENCE,
	asn_DEF_X2N_ProtectedResourceList_Item_tags_1,
	sizeof(asn_DEF_X2N_ProtectedResourceList_Item_tags_1)
		/sizeof(asn_DEF_X2N_ProtectedResourceList_Item_tags_1[0]), /* 1 */
	asn_DEF_X2N_ProtectedResourceList_Item_tags_1,	/* Same as above */
	sizeof(asn_DEF_X2N_ProtectedResourceList_Item_tags_1)
		/sizeof(asn_DEF_X2N_ProtectedResourceList_Item_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_X2N_ProtectedResourceList_Item_1,
	5,	/* Elements count */
	&asn_SPC_X2N_ProtectedResourceList_Item_specs_1	/* Additional specs */
};

