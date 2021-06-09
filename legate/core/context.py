# Copyright 2021 NVIDIA Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from .legion import legion


class ResourceConfig(object):
    __slots__ = [
        "max_tasks",
        "max_mappers",
        "max_reduction_ops",
        "max_projections",
        "max_shardings",
    ]

    def __init__(self):
        self.max_tasks = 1_000_000
        self.max_mappers = 1
        self.max_reduction_ops = 0
        self.max_projections = 0
        self.max_shardings = 0


class ResourceScope(object):
    def __init__(self, context, base, category):
        self._context = context
        self._base = base
        self._category = category

    @property
    def scope(self):
        return self._context._library.name

    def translate(self, resource_id):
        if self._base is None:
            raise ValueError(f"{self.scope} has not {self._category}")
        return self._base + resource_id


class Context(object):
    def __init__(self, runtime, library):
        """
        A Context is a named scope for Legion resources used in a Legate
        library. A Context is created when the library is registered
        for the first time to the Legate runtime, and it must be passed
        when the library registers or makes accesses to its Legion resources.
        Resources that are scoped locally to each library include
        task ids, projection and sharding functor ids, and reduction operator
        ids.
        """
        self._runtime = runtime
        self._library = library

        config = library.get_resource_configuration()
        name = library.get_name().encode("utf-8")
        lg_runtime = self._runtime.legion_runtime

        def _create_scope(api, category, max_counts):
            base = (
                api(lg_runtime, name, max_counts) if max_counts > 0 else None
            )
            return ResourceScope(self, base, category)

        self._task_scope = _create_scope(
            legion.legion_runtime_generate_library_task_ids,
            "task",
            config.max_tasks,
        )
        self._mapper_scope = _create_scope(
            legion.legion_runtime_generate_library_mapper_ids,
            "mapper",
            config.max_mappers,
        )
        self._redop_scope = _create_scope(
            legion.legion_runtime_generate_library_reduction_ids,
            "reduction op",
            config.max_reduction_ops,
        )
        self._proj_scope = _create_scope(
            legion.legion_runtime_generate_library_projection_ids,
            "Projection functor",
            config.max_projections,
        )
        self._shard_scope = _create_scope(
            legion.legion_runtime_generate_library_sharding_ids,
            "sharding functor",
            config.max_shardings,
        )

    def destroy(self):
        self._library.destroy()

    @property
    def first_mapper_id(self):
        return self._mapper_scope._base

    @property
    def first_redop_id(self):
        return self._redop_scope._base

    @property
    def first_shard_id(self):
        return self._shard_scope._base

    @property
    def empty_argmap(self):
        return self._runtime.empty_argmap

    def get_task_id(self, task_id):
        return self._task_scope.translate(task_id)

    def get_mapper_id(self, mapper_id):
        return self._mapper_scope.translate(mapper_id)

    def get_reduction_op_id(self, redop_id):
        return self._redop_scope.translate(redop_id)

    def get_projection_id(self, proj_id):
        if proj_id == 0:
            return proj_id
        else:
            return self._proj_scope.translate(proj_id)

    def get_sharding_id(self, shard_id):
        return self._shard_scope.translate(shard_id)

    def dispatch(self, op, redop=None):
        return self._runtime.dispatch(op, redop)
