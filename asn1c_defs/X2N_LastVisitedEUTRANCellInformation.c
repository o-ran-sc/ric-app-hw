/*
 * Generated by asn1c-0.9.29 n1 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-15-04.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#include "X2N_LastVisitedEUTRANCellInformation.h"

#include "X2N_ProtocolExtensionContainer.h"
asn_TYPE_member_t asn_MBR_X2N_LastVisitedEUTRANCellInformation_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_LastVisitedEUTRANCellInformation, global_Cell_ID),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ECGI,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"global-Cell-ID"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_LastVisitedEUTRANCellInformation, cellType),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_CellType,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cellType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_LastVisitedEUTRANCellInformation, time_UE_StayedInCell),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_Time_UE_StayedInCell,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"time-UE-StayedInCell"
		},
	{ ATF_POINTER, 1, offsetof(struct X2N_LastVisitedEUTRANCellInformation, iE_Extensions),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ProtocolExtensionContainer_8231P157,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"iE-Extensions"
		},
};
static const int asn_MAP_X2N_LastVisitedEUTRANCellInformation_oms_1[] = { 3 };
static const ber_tlv_tag_t asn_DEF_X2N_LastVisitedEUTRANCellInformation_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_X2N_LastVisitedEUTRANCellInformation_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* global-Cell-ID */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* cellType */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* time-UE-StayedInCell */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* iE-Extensions */
};
asn_SEQUENCE_specifics_t asn_SPC_X2N_LastVisitedEUTRANCellInformation_specs_1 = {
	sizeof(struct X2N_LastVisitedEUTRANCellInformation),
	offsetof(struct X2N_LastVisitedEUTRANCellInformation, _asn_ctx),
	asn_MAP_X2N_LastVisitedEUTRANCellInformation_tag2el_1,
	4,	/* Count of tags in the map */
	asn_MAP_X2N_LastVisitedEUTRANCellInformation_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	4,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_X2N_LastVisitedEUTRANCellInformation = {
	"LastVisitedEUTRANCellInformation",
	"LastVisitedEUTRANCellInformation",
	&asn_OP_SEQUENCE,
	asn_DEF_X2N_LastVisitedEUTRANCellInformation_tags_1,
	sizeof(asn_DEF_X2N_LastVisitedEUTRANCellInformation_tags_1)
		/sizeof(asn_DEF_X2N_LastVisitedEUTRANCellInformation_tags_1[0]), /* 1 */
	asn_DEF_X2N_LastVisitedEUTRANCellInformation_tags_1,	/* Same as above */
	sizeof(asn_DEF_X2N_LastVisitedEUTRANCellInformation_tags_1)
		/sizeof(asn_DEF_X2N_LastVisitedEUTRANCellInformation_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_X2N_LastVisitedEUTRANCellInformation_1,
	4,	/* Elements count */
	&asn_SPC_X2N_LastVisitedEUTRANCellInformation_specs_1	/* Additional specs */
};

