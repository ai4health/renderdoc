#pragma once

struct DDS_info {
	string file;
	int org_len;
	int len;
	void *org_buff;
	void *buff;
	vector<pair<void *, int>> layer_vec;
};

const vector<pair<DDS_info, DDS_info>> *af_get_replace_vector();
const vector<DDS_info> *af_get_process_vector();

void af_add_trace_resource_id(uint64_t);

const set<uint64_t> *af_get_trace_resource_ids();

void af_add_save_event_id(uint32_t);
void af_add_repeat_event_id(uint32_t);
bool is_exist_event_id(uint32_t id);

void af_open_scan_event();
void af_close_scan_event();
bool af_check_scan_event();
