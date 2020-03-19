/*
 * Generated by asn1c-0.9.29 n1 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-PDU-Contents"
 * 	found in "../../asn_defs/asn1/x2ap-15-04.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#include "X2N_GNBStatusIndication.h"

asn_TYPE_member_t asn_MBR_X2N_GNBStatusIndication_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct X2N_GNBStatusIndication, protocolIEs),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_X2N_ProtocolIE_Container_8180P105,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"protocolIEs"
		},
};
static const ber_tlv_tag_t asn_DEF_X2N_GNBStatusIndication_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_X2N_GNBStatusIndication_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* protocolIEs */
};
asn_SEQUENCE_specifics_t asn_SPC_X2N_GNBStatusIndication_specs_1 = {
	sizeof(struct X2N_GNBStatusIndication),
	offsetof(struct X2N_GNBStatusIndication, _asn_ctx),
	asn_MAP_X2N_GNBStatusIndication_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_X2N_GNBStatusIndication = {
	"GNBStatusIndication",
	"GNBStatusIndication",
	&asn_OP_SEQUENCE,
	asn_DEF_X2N_GNBStatusIndication_tags_1,
	sizeof(asn_DEF_X2N_GNBStatusIndication_tags_1)
		/sizeof(asn_DEF_X2N_GNBStatusIndication_tags_1[0]), /* 1 */
	asn_DEF_X2N_GNBStatusIndication_tags_1,	/* Same as above */
	sizeof(asn_DEF_X2N_GNBStatusIndication_tags_1)
		/sizeof(asn_DEF_X2N_GNBStatusIndication_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_X2N_GNBStatusIndication_1,
	1,	/* Elements count */
	&asn_SPC_X2N_GNBStatusIndication_specs_1	/* Additional specs */
};

