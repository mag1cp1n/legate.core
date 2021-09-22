/* Copyright 2021 NVIDIA Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include <memory>

#include "legion.h"

#include "data/scalar.h"
#include "mapping/mapping.h"
#include "runtime/context.h"
#include "utilities/typedefs.h"

namespace legate {
namespace mapping {

class InstanceManager;

enum class Strictness : bool {
  strict = true,
  hint   = false,
};

class BaseMapper : public Legion::Mapping::Mapper, public LegateMapper {
 public:
  BaseMapper(Legion::Mapping::MapperRuntime* rt,
             Legion::Machine machine,
             const LibraryContext& context);
  virtual ~BaseMapper(void);

 private:
  BaseMapper(const BaseMapper& rhs) = delete;
  BaseMapper& operator=(const BaseMapper& rhs) = delete;

 protected:
  // Start-up methods
  static Legion::AddressSpaceID get_local_node(void);
  static size_t get_total_nodes(Legion::Machine m);
  std::string create_name(Legion::AddressSpace node) const;
  std::string create_logger_name() const;

 public:
  virtual const char* get_mapper_name(void) const override;
  virtual Legion::Mapping::Mapper::MapperSyncModel get_mapper_sync_model(void) const;
  virtual bool request_valid_instances(void) const { return false; }

 public:  // Task mapping calls
  virtual void select_task_options(const Legion::Mapping::MapperContext ctx,
                                   const Legion::Task& task,
                                   TaskOptions& output);
  virtual void premap_task(const Legion::Mapping::MapperContext ctx,
                           const Legion::Task& task,
                           const PremapTaskInput& input,
                           PremapTaskOutput& output);
  virtual void slice_task(const Legion::Mapping::MapperContext ctx,
                          const Legion::Task& task,
                          const SliceTaskInput& input,
                          SliceTaskOutput& output);
  virtual void map_task(const Legion::Mapping::MapperContext ctx,
                        const Legion::Task& task,
                        const MapTaskInput& input,
                        MapTaskOutput& output);
  virtual void map_replicate_task(const Legion::Mapping::MapperContext ctx,
                                  const Legion::Task& task,
                                  const MapTaskInput& input,
                                  const MapTaskOutput& default_output,
                                  MapReplicateTaskOutput& output);
  virtual void select_task_variant(const Legion::Mapping::MapperContext ctx,
                                   const Legion::Task& task,
                                   const SelectVariantInput& input,
                                   SelectVariantOutput& output);
  virtual void postmap_task(const Legion::Mapping::MapperContext ctx,
                            const Legion::Task& task,
                            const PostMapInput& input,
                            PostMapOutput& output);
  virtual void select_task_sources(const Legion::Mapping::MapperContext ctx,
                                   const Legion::Task& task,
                                   const SelectTaskSrcInput& input,
                                   SelectTaskSrcOutput& output);
  virtual void speculate(const Legion::Mapping::MapperContext ctx,
                         const Legion::Task& task,
                         SpeculativeOutput& output);
  virtual void report_profiling(const Legion::Mapping::MapperContext ctx,
                                const Legion::Task& task,
                                const TaskProfilingInfo& input);
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::Task& task,
                                       const SelectShardingFunctorInput& input,
                                       SelectShardingFunctorOutput& output);

 public:  // Inline mapping calls
  virtual void map_inline(const Legion::Mapping::MapperContext ctx,
                          const Legion::InlineMapping& inline_op,
                          const MapInlineInput& input,
                          MapInlineOutput& output);
  virtual void select_inline_sources(const Legion::Mapping::MapperContext ctx,
                                     const Legion::InlineMapping& inline_op,
                                     const SelectInlineSrcInput& input,
                                     SelectInlineSrcOutput& output);
  virtual void report_profiling(const Legion::Mapping::MapperContext ctx,
                                const Legion::InlineMapping& inline_op,
                                const InlineProfilingInfo& input);

 public:  // Copy mapping calls
  virtual void map_copy(const Legion::Mapping::MapperContext ctx,
                        const Legion::Copy& copy,
                        const MapCopyInput& input,
                        MapCopyOutput& output);
  virtual void select_copy_sources(const Legion::Mapping::MapperContext ctx,
                                   const Legion::Copy& copy,
                                   const SelectCopySrcInput& input,
                                   SelectCopySrcOutput& output);
  virtual void speculate(const Legion::Mapping::MapperContext ctx,
                         const Legion::Copy& copy,
                         SpeculativeOutput& output);
  virtual void report_profiling(const Legion::Mapping::MapperContext ctx,
                                const Legion::Copy& copy,
                                const CopyProfilingInfo& input);
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::Copy& copy,
                                       const SelectShardingFunctorInput& input,
                                       SelectShardingFunctorOutput& output);

 public:  // Close mapping calls
  virtual void map_close(const Legion::Mapping::MapperContext ctx,
                         const Legion::Close& close,
                         const MapCloseInput& input,
                         MapCloseOutput& output);
  virtual void select_close_sources(const Legion::Mapping::MapperContext ctx,
                                    const Legion::Close& close,
                                    const SelectCloseSrcInput& input,
                                    SelectCloseSrcOutput& output);
  virtual void report_profiling(const Legion::Mapping::MapperContext ctx,
                                const Legion::Close& close,
                                const CloseProfilingInfo& input);
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::Close& close,
                                       const SelectShardingFunctorInput& input,
                                       SelectShardingFunctorOutput& output);

 public:  // Acquire mapping calls
  virtual void map_acquire(const Legion::Mapping::MapperContext ctx,
                           const Legion::Acquire& acquire,
                           const MapAcquireInput& input,
                           MapAcquireOutput& output);
  virtual void speculate(const Legion::Mapping::MapperContext ctx,
                         const Legion::Acquire& acquire,
                         SpeculativeOutput& output);
  virtual void report_profiling(const Legion::Mapping::MapperContext ctx,
                                const Legion::Acquire& acquire,
                                const AcquireProfilingInfo& input);
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::Acquire& acquire,
                                       const SelectShardingFunctorInput& input,
                                       SelectShardingFunctorOutput& output);

 public:  // Release mapping calls
  virtual void map_release(const Legion::Mapping::MapperContext ctx,
                           const Legion::Release& release,
                           const MapReleaseInput& input,
                           MapReleaseOutput& output);
  virtual void select_release_sources(const Legion::Mapping::MapperContext ctx,
                                      const Legion::Release& release,
                                      const SelectReleaseSrcInput& input,
                                      SelectReleaseSrcOutput& output);
  virtual void speculate(const Legion::Mapping::MapperContext ctx,
                         const Legion::Release& release,
                         SpeculativeOutput& output);
  virtual void report_profiling(const Legion::Mapping::MapperContext ctx,
                                const Legion::Release& release,
                                const ReleaseProfilingInfo& input);
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::Release& release,
                                       const SelectShardingFunctorInput& input,
                                       SelectShardingFunctorOutput& output);

 public:  // Partition mapping calls
  virtual void select_partition_projection(const Legion::Mapping::MapperContext ctx,
                                           const Legion::Partition& partition,
                                           const SelectPartitionProjectionInput& input,
                                           SelectPartitionProjectionOutput& output);
  virtual void map_partition(const Legion::Mapping::MapperContext ctx,
                             const Legion::Partition& partition,
                             const MapPartitionInput& input,
                             MapPartitionOutput& output);
  virtual void select_partition_sources(const Legion::Mapping::MapperContext ctx,
                                        const Legion::Partition& partition,
                                        const SelectPartitionSrcInput& input,
                                        SelectPartitionSrcOutput& output);
  virtual void report_profiling(const Legion::Mapping::MapperContext ctx,
                                const Legion::Partition& partition,
                                const PartitionProfilingInfo& input);
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::Partition& partition,
                                       const SelectShardingFunctorInput& input,
                                       SelectShardingFunctorOutput& output);

 public:  // Fill mapper calls
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::Fill& fill,
                                       const SelectShardingFunctorInput& input,
                                       SelectShardingFunctorOutput& output);

 public:  // Task execution mapping calls
  virtual void configure_context(const Legion::Mapping::MapperContext ctx,
                                 const Legion::Task& task,
                                 ContextConfigOutput& output);
  virtual void select_tunable_value(const Legion::Mapping::MapperContext ctx,
                                    const Legion::Task& task,
                                    const SelectTunableInput& input,
                                    SelectTunableOutput& output);

 public:  // Must epoch mapping
  virtual void select_sharding_functor(const Legion::Mapping::MapperContext ctx,
                                       const Legion::MustEpoch& epoch,
                                       const SelectShardingFunctorInput& input,
                                       MustEpochShardingFunctorOutput& output);
  virtual void memoize_operation(const Legion::Mapping::MapperContext ctx,
                                 const Legion::Mappable& mappable,
                                 const MemoizeInput& input,
                                 MemoizeOutput& output);
  virtual void map_must_epoch(const Legion::Mapping::MapperContext ctx,
                              const MapMustEpochInput& input,
                              MapMustEpochOutput& output);

 public:  // Dataflow graph mapping
  virtual void map_dataflow_graph(const Legion::Mapping::MapperContext ctx,
                                  const MapDataflowGraphInput& input,
                                  MapDataflowGraphOutput& output);

 public:  // Mapping control and stealing
  virtual void select_tasks_to_map(const Legion::Mapping::MapperContext ctx,
                                   const SelectMappingInput& input,
                                   SelectMappingOutput& output);
  virtual void select_steal_targets(const Legion::Mapping::MapperContext ctx,
                                    const SelectStealingInput& input,
                                    SelectStealingOutput& output);
  virtual void permit_steal_request(const Legion::Mapping::MapperContext ctx,
                                    const StealRequestInput& intput,
                                    StealRequestOutput& output);

 public:  // handling
  virtual void handle_message(const Legion::Mapping::MapperContext ctx,
                              const MapperMessage& message);
  virtual void handle_task_result(const Legion::Mapping::MapperContext ctx,
                                  const MapperTaskResult& result);

 protected:
  Legion::Memory get_target_memory(Legion::Processor proc, StoreTarget target);
  bool find_existing_instance(Legion::LogicalRegion region,
                              Legion::FieldID fid,
                              Legion::Memory target_memory,
                              Legion::Mapping::PhysicalInstance& result,
                              Strictness strictness = Strictness::hint);
  bool map_legate_store(const Legion::Mapping::MapperContext ctx,
                        const Legion::Mappable& mappable,
                        const StoreMapping& mapping,
                        const Legion::RegionRequirement& req,
                        Legion::Processor target_proc,
                        const std::vector<Legion::Mapping::PhysicalInstance>& valid,
                        Legion::Mapping::PhysicalInstance& result);
  bool map_raw_array(const Legion::Mapping::MapperContext ctx,
                     const Legion::Mappable& mappable,
                     unsigned index,
                     Legion::LogicalRegion region,
                     Legion::FieldID fid,
                     Legion::Memory target_memory,
                     Legion::Processor target_proc,
                     const std::vector<Legion::Mapping::PhysicalInstance>& valid,
                     Legion::Mapping::PhysicalInstance& result,
                     bool memoize,
                     Legion::ReductionOpID redop = 0);
  void filter_failed_acquires(std::vector<Legion::Mapping::PhysicalInstance>& needed_acquires,
                              std::set<Legion::Mapping::PhysicalInstance>& failed_acquires);
  void report_failed_mapping(const Legion::Mappable& mappable,
                             unsigned index,
                             Legion::Memory target_memory,
                             Legion::ReductionOpID redop);
  void legate_select_sources(const Legion::Mapping::MapperContext ctx,
                             const Legion::Mapping::PhysicalInstance& target,
                             const std::vector<Legion::Mapping::PhysicalInstance>& sources,
                             std::deque<Legion::Mapping::PhysicalInstance>& ranking);
  bool has_variant(const Legion::Mapping::MapperContext ctx,
                   const Legion::Task& task,
                   Legion::Processor::Kind kind);
  Legion::VariantID find_variant(const Legion::Mapping::MapperContext ctx,
                                 const Legion::Task& task,
                                 Legion::Processor::Kind kind);

 protected:
  static inline bool physical_sort_func(
    const std::pair<Legion::Mapping::PhysicalInstance, unsigned>& left,
    const std::pair<Legion::Mapping::PhysicalInstance, unsigned>& right)
  {
    return (left.second < right.second);
  }
  // NumPyOpCode decode_task_id(Legion::TaskID tid);

 public:
  const Legion::Machine machine;
  const LibraryContext context;
  const Legion::AddressSpace local_node;
  const size_t total_nodes;
  const std::string mapper_name;
  Legion::Logger logger;

 protected:
  std::vector<Legion::Processor> local_cpus;
  std::vector<Legion::Processor> local_gpus;
  std::vector<Legion::Processor> local_omps;  // OpenMP processors
  std::vector<Legion::Processor> local_ios;   // I/O processors
  std::vector<Legion::Processor> local_pys;   // Python processors
 protected:
  Legion::Memory local_system_memory, local_zerocopy_memory;
  std::map<Legion::Processor, Legion::Memory> local_frame_buffers;
  std::map<Legion::Processor, Legion::Memory> local_numa_domains;

 protected:
  std::map<std::pair<Legion::TaskID, Legion::Processor::Kind>, Legion::VariantID> leaf_variants;

 protected:
  std::unique_ptr<InstanceManager> local_instances;

 protected:
  // These are used for computing sharding functions
  std::map<Legion::IndexPartition, unsigned> partition_color_space_dims;
  std::map<Legion::IndexSpace, unsigned> index_color_dims;
};

}  // namespace mapping
}  // namespace legate
